var JASPWidgets = {};

JASPWidgets.Encodings = {
	getBase64: function (arrayBuffer) {
		var base64 = ''
		var encodings = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'

		var bytes = new Uint8Array(arrayBuffer)
		var byteLength = bytes.byteLength
		var byteRemainder = byteLength % 3
		var mainLength = byteLength - byteRemainder

		var a, b, c, d
		var chunk

		// Main loop deals with bytes in chunks of 3
		for (var i = 0; i < mainLength; i = i + 3) {
			// Combine the three bytes into a single integer
			chunk = (bytes[i] << 16) | (bytes[i + 1] << 8) | bytes[i + 2]

			// Use bitmasks to extract 6-bit segments from the triplet
			a = (chunk & 16515072) >> 18 // 16515072 = (2^6 - 1) << 18
			b = (chunk & 258048) >> 12 // 258048   = (2^6 - 1) << 12
			c = (chunk & 4032) >> 6 // 4032     = (2^6 - 1) << 6
			d = chunk & 63               // 63       = 2^6 - 1

			// Convert the raw binary segments to the appropriate ASCII encoding
			base64 += encodings[a] + encodings[b] + encodings[c] + encodings[d]
		}

		// Deal with the remaining bytes and padding
		if (byteRemainder == 1) {
			chunk = bytes[mainLength]

			a = (chunk & 252) >> 2 // 252 = (2^6 - 1) << 2

			// Set the 4 least significant bits to zero
			b = (chunk & 3) << 4 // 3   = 2^2 - 1

			base64 += encodings[a] + encodings[b] + '=='
		} else if (byteRemainder == 2) {
			chunk = (bytes[mainLength] << 8) | bytes[mainLength + 1]

			a = (chunk & 64512) >> 10 // 64512 = (2^6 - 1) << 10
			b = (chunk & 1008) >> 4 // 1008  = (2^6 - 1) << 4

			// Set the 2 least significant bits to zero
			c = (chunk & 15) << 2 // 15    = 2^4 - 1

			base64 += encodings[a] + encodings[b] + encodings[c] + '='
		}

		return base64
	},


}

JASPWidgets.ExportProperties = {

	format: {
		raw: 0,
		html: 1, //bit 1
		formatted: 2, //bit 2
		formattedHTML: 3
	},

	process: {
		copy: 0,
		save: 1
	},

	htmlImageFormat: {
		temporary: 0,
		resource: 1,
		embedded: 2
	}
}

JASPWidgets.Exporter = {

	params: function () {

		this.format = JASPWidgets.ExportProperties.format.raw,
		this.process = JASPWidgets.ExportProperties.process.copy,
		this.htmlImageFormat = JASPWidgets.ExportProperties.htmlImageFormat.temporary,
		this.includeNotes = false;

		this.isFormatted = function () {
			return (this.format & JASPWidgets.ExportProperties.format.formatted) === JASPWidgets.ExportProperties.format.formatted
		};

		this.htmlOnly = function () {
			return (this.format & JASPWidgets.ExportProperties.format.html) === JASPWidgets.ExportProperties.format.html
		};

		this.clone = function () {
			var cloneParams = new JASPWidgets.Exporter.params();
			cloneParams.format = this.format;
			cloneParams.process = this.process;
			cloneParams.htmlImageFormat = this.htmlImageFormat;
			cloneParams.includeNotes = this.includeNotes;
			return cloneParams;
		};
	},

	data: function (raw, html) {

		if (raw == null)
			raw = "";

		this.raw = raw;
		this.html = html;
	},

	begin: function (exportObj, exportParams, completedCallback, useNBSP, innerStyle) {

		if (innerStyle === undefined)
			innerStyle = "";

		if (useNBSP == undefined)
			useNBSP = false;

		if (exportParams == undefined)
			exportParams = new JASPWidgets.Exporter.params();
		else if (exportParams.error)
			return false;

		if (exportObj.views) {
			var viewList = [];
			for (var i = 0; i < exportObj.views.length; i++) {
				var view = exportObj.views[i];
				if (exportParams.includeNotes || view.$el.hasClass('jasp-notes') === false)
					viewList.push(view);
			}

			if (viewList.length === 0)
				completedCallback.call(exportObj, exportParams, new JASPWidgets.Exporter.data(null, ""));
			else {
				exportObj.buffer = [];
				exportObj.exportCounter = viewList.length;

				for (var i = 0; i < viewList.length; i++)
					this._exportView(exportParams, viewList[i], i, exportObj, useNBSP, innerStyle, completedCallback);
			}
		}
		else if (exportObj.exportBegin) {
			exportObj.exportBegin(exportParams, completedCallback);
		} else
			return false;

		return true;
	},

	_exportView: function (exportParams, view, i, parent, useNBSP, innerStyle, completedCallback) {

		var self = parent;
		var index = i;
		var callback = completedCallback;
		var trackerView = view;
		var cc = function (exParams, exContent) {
			self.buffer[index] = exContent;
			self.exportCounter -= 1;
			if (self.exportCounter === 0) {
				var completeText = "";
				var raw = null;
				if (!exportParams.error) {
					completeText = "<div " + self.getStyleAttr() + ">\n";
					completeText += '<div style="display:inline-block; ' + innerStyle + '">\n';
					if (!self.disableTitleExport && self.toolbar !== undefined) {
						completeText += JASPWidgets.Exporter.getTitleHtml(self.toolbar, exportParams)
					}
					var firstItem = true;
					for (var j = 0; j < self.buffer.length; j++) {
						if (self.buffer[j]) {
							if (self.buffer[j].raw !== null && self.buffer[j].raw !== '')
								raw = self.buffer[j].raw;

							var bufferHtml = self.buffer[j].html;
							if (bufferHtml !== '') {
								var includeSpacer = false;
								if (exParams.format !== JASPWidgets.ExportProperties.format.formattedHTML) {
									if (firstItem === false && (JASPWidgets.Exporter.isInlineStyle(self.views[j - 1].$el) == false)) {
										if ((this.hasExportNSBFOverride && this.hasExportNSBFOverride()) || this.useExportNSBF)
											includeSpacer = this.useExportNSBF();
										else
											includeSpacer = useNBSP;
									}
								}

								if (includeSpacer)
									completeText += "&nbsp;";

								completeText += self.buffer[j].html;
								firstItem = false;
							}
						}
					}
					completeText += "</div>";
					completeText += "</div>";
				}

				if (parent.exportWrapper)
					completeText = parent.exportWrapper(completeText);

				callback.call(self, exportParams, new JASPWidgets.Exporter.data(raw, completeText));
				self.buffer = [];
			}
		}
		view.exportBegin(exportParams, cc);
	},

	getStyles: function (element, styleItems) {

		var css = element.css(styleItems);
		if (css === undefined)
			return "";

		var style = "";
		for (var i = 0; i < styleItems.length; i++)
		{
			var styleItem = styleItems[i];
			if (css[styleItem])
				style += styleItem + ": " + css[styleItem] + "; "
		}

		if (style)
			style = 'style="' + style + '"';

		return style;
	},

	getSpacingStyles: function (element, exportParams) {
		if (exportParams.isFormatted())
			return JASPWidgets.Exporter.getStyles(element, ["padding", "margin", "display", "float"]);
		else
			return "";//JASPWidgets.Exporter.getStyles(element, ["display", "float"]);
	},

	getHeaderStyles: function (element, exportParams) {
		if (exportParams.isFormatted())
			return JASPWidgets.Exporter.getStyles(element, ["padding", "text-align", "margin", "display", "float", "vertical-align", "font-size", "font", "font-weight", "color"]);
		else
			return "";//JASPWidgets.Exporter.getStyles(element, ["display", "float"]);
	},

	getTableStyles: function (element, exportParams) {
		if (exportParams.isFormatted())
			return JASPWidgets.Exporter.getStyles(element, ["border-collapse", "border-top-width", "border-bottom-width", "border-left-width", "border-right-width", "border-color", "border-style", "padding", "text-align", "margin-bottom", "margin-top", "display", "float", "color"]);
		else
			return JASPWidgets.Exporter.getStyles(element, ["border-collapse", "border-top-width", "border-bottom-width", "border-left-width", "border-right-width", "border-color", "border-style", "display", "float"]);
	},

	getTableContentStyles: function (element, exportParams) {
		if (exportParams.isFormatted())
			return JASPWidgets.Exporter.getStyles(element, ["border-collapse", "border-top-width", "border-bottom-width", "border-left-width", "border-right-width", "border-color", "border-style", "padding", "text-align", "margin", "display", "float", "font-size", "font-weight", "font", "color"]);
		else
			return JASPWidgets.Exporter.getStyles(element, ["border-collapse", "border-top-width", "border-bottom-width", "border-left-width", "border-right-width", "border-color", "border-style", "display", "float", "text-align"]);
	},

	getErrorStyles: function (element, component) {
		if (component === "error-message-positioner")
			return JASPWidgets.Exporter.getStyles(element, ["padding", "margin", "display", "float", "color", "height", "overflow", "position", "top", "z-index"]);
		else if (component === "error-message-box")
			return JASPWidgets.Exporter.getStyles(element, ["margin", "border", "background-color", "color", "padding", "display", "float", "border-radius", "min-width", "max-width", "white-space"]);
		else if (component === "error-message-symbol ")
			return JASPWidgets.Exporter.getStyles(element, ["margin", "border", "background-color", "color", "padding", "display", "float"]);
		else
			return JASPWidgets.Exporter.getStyles(element, ["margin", "border", "background-color", "color", "padding", "text-align", "display", "float", "vertical-align", "font-size", "font", "font-weight"]);
	},

	getNoteStyles: function (element, exportParams) {
		if (exportParams.isFormatted())
			return JASPWidgets.Exporter.getStyles(element, ["margin", "padding", "max-width", "min-width", "display", "font-size", "font-weight", "font", "color", "border-top-style", "border-top-width", "border-top-color", "border-bottom-style", "border-bottom-width", "border-bottom-color"]);
		else
			return JASPWidgets.Exporter.getStyles(element, ["max-width", "min-width"]);
	},

	exportErrorWindow: function (element, error) {
		var text = "";

		if (error && error.errorMessage) {

			text += '<div ' + JASPWidgets.Exporter.getErrorStyles(element, 'error-message-positioner') + '>'
			text += '<div ' + JASPWidgets.Exporter.getErrorStyles(element.find('.error-message-box'), 'error-message-box') + '>'
			text += '<span ' + JASPWidgets.Exporter.getErrorStyles(element.find('.error-message-symbol'), 'error-message-symbol') + '></span>'
			text += '<div ' + JASPWidgets.Exporter.getErrorStyles(element.find('.error-message-message'), 'error-message-message') + '>' + error.errorMessage + '</div>'
			text += '</div>'
			text += '</div>'
		}

		return text;
	},

	isInlineStyle: function (element) {
		var css = element.css("display");

		return css === "inline" || css === "inline-block";
	},

	getTitleHtml: function (toolbar, exportParams) {
		var html = toolbar.title === undefined ? "" : toolbar.title;
		if (toolbar.titleTag !== undefined) {
			var headerStyles = " " + JASPWidgets.Exporter.getHeaderStyles(toolbar.$title(), exportParams);
			html = '<' + toolbar.titleTag + headerStyles + '>' + toolbar.title + '</' + toolbar.titleTag + '>';
		}

		var topLevelStyles = " " + JASPWidgets.Exporter.getSpacingStyles(toolbar.$el, exportParams);
		return '<div ' + topLevelStyles + '>' + html + '</div>';
	},
};

JASPWidgets.View = Backbone.View.extend({

	inCollection: false,

	getStyleAttr: function (styleItems) {

		return JASPWidgets.Exporter.getStyles(this.$el, ["padding", "text-align", "margin-bottom", "margin-top", "margin-left", "margin-right", "display", "float", "position"]);

	},

	_isClosed: false,
	/** Removes DOM tree elements and any listeners from or too the view object */
	close: function () {
		if (!this._isClosed) {
			if (this.onClose)
				this.onClose();

			this.remove();
			this.off();
			this._isClosed = true;
		}
	},
})

JASPWidgets.Note = Backbone.Model.extend({
	defaults: {
		text: '<p><br></p>',
		format: 'markdown',
		delta: {},
		deltaAvailable: false,
	},

	toHtml: function () {
		if (this.get('format') === 'markdown') {
			this.set('format', 'html');
			var text = this.get('text');
			if (text === null || text === '')
				this.set('text', '<p><br></p>');
			else if (text !== '')
				this.set('text', Mrkdwn.toHtml(text));
		}
	},
});

JASPWidgets.NoteBox = JASPWidgets.View.extend({

	//#7C95CB
	//#F2F7FD

	initialize: function () {

		this.ghostTextDefault = 'Click here to add text...';

		if (JASPWidgets.NoteBox.activeNoteBox === undefined)
			JASPWidgets.NoteBox.activeNoteBox = null;

		this.editing = false;
		this.ghostTextVisible = true;

		this.visible = this.model.get('visible');
		if (this.visible === undefined || this.visible === null)
			this.visible = false;

		this.internalChange = false;

		if (this.model.get('format') === 'markdown')
			this.model.toHtml();

		// this.listenTo(this.model, 'change:text', this.textChanged)

		this.closeButton = new JASPWidgets.ActionView({ className: "jasp-closer" });
		var self = this;
		this.closeButton.actionTargetElement = function () {
			return self.$el;
		};
		this.closeButton.setAction(function () {
			self.setVisibilityAnimate(false);
			if (window.resultsDocumentChanged)
				window.resultsDocumentChanged();
		});
	},

	events: {
		'mouseenter': '_hoveringStart',
		'mouseleave': '_hoveringEnd',
		'click'     : '_mouseClicked',
	},

	detach: function() {
		this.$el.detach();
		this.closeButton.$el.detach();
	},

	_hoveringStart: function (e) {
		this.closeButton.setVisibility(true);

		if (!this.$quill.hasFocus()) {
			this.setQuillToolbarVisibility('none');
		}
	},

	_hoveringEnd: function (e) {
		this.closeButton.setVisibility(false);

		if (!this.$quill.hasFocus()) {
			this.setQuillToolbarVisibility('none');
		}
	},

	_mouseClicked: function (e) {
		this.setQuillToolbarVisibility('block');
	},

	clear: function () {

		this.model.set('format', 'html');
		this.model.set('text', '');
		this.model.set('delta', {});
		this.model.set('deltaAvailable', false);
	},

	setGhostTextVisible: function(visible) {
		this.ghostTextVisible = visible;
	},

	isTextboxEmpty: function () {
		return this.$quill.getLength() === 0;
	},

	render: function () {

		if (this._inited) {
			this.$quill.off();
			delete this.$quill;
		}

		this.$el.empty();

		this.setVisibility(this.visible)

		var html = this.model.get("text");

		this.closeButton.render();

		this.$el.append('<div class="jasp-hide" data-button-class="jasp-comment"></div>');
		this.$el.append("<div id=\"editor\"></div>");

		var toolbarOptions = [
			['bold', 'italic', 'underline'],  // image
			// [{ 'size': ['small', false, 'large', 'huge'] }],
			[{ 'header': [1, 2, 3, 4, false] }],
			[{ 'list': 'ordered'}, { 'list': 'bullet' }],
			[{ 'color': [] }, { 'background': [] }],
			[{ 'script': 'sub'}, { 'script': 'super' }],
			['blockquote', { 'indent': '-1'}, { 'indent': '+1' }],
			// [{ 'font': [] }, { 'align': [] }],
			['clean']
		];

		let targetDiv = this.$el.find("#editor").get(0)
		this.$quill = new Quill(targetDiv, {
			modules: {
				toolbar: toolbarOptions
			},
			theme: 'snow'
		});

		var self = this;
		var delt;

		if (this.model.get('deltaAvailable')) {
			delt = this.model.get('delta');
		} else {
			if (this.model.get('format') === 'markdown') {
				this.model.toHtml();
				html = this.model.get("text");
			}
			delt = this.$quill.clipboard.convert(html);
		}
		this.$quill.setContents(delt);
		self.onNoteChanged(self.$quill.root.innerHTML, self.$quill.getContents());

		this.$quill.on('text-change', function(delta, oldDelta, source) {
			self.onNoteChanged(self.$quill.root.innerHTML, self.$quill.getContents());
		});

		this.$quillToolbar = this.$el.find(".ql-toolbar").get(0)

		this.quillToolbarClicked = false;
		this.$quillToolbar.addEventListener('click', function() {

			this.quillToolbarClicked = true;
		}, false);

		this.setQuillToolbarVisibility('none');

		this._inited = true;

		return this;
	},

	onNoteChanged: function (html, quDelta) {

		this.internalChange = true;

		this.model.set({
			'text': html,
			'format': 'html',
			'delta': quDelta,
			'deltaAvailable' : true
		});

		this.internalChange = false;
	},

	setQuillToolbarVisibility: function(display) {
		// display: ['block', 'none']

		this.$quillToolbar.style.display = display;
	},

	setVisibility: function(value) {
		this.visible = value;

		if (value)
			this.$el.removeClass('jasp-hide');
		else
			this.$el.addClass('jasp-hide');
	},

	setVisibilityAnimate: function (value, scroll) {

		var self = this;
		var scrollIntoView = scroll === undefined ? true : scroll;
		self.$el.css("opacity", value ? 0 : 1);

		if (value === true) {
			self.$el.slideDown(200, function () {
				self.setVisibility(value);
				if (scrollIntoView)
					self.setGhostTextVisible(false);
				self.$el.animate({ "opacity": 1 }, 200, "easeOutCubic", function () {
					if (scrollIntoView) {
						window.scrollIntoView(self.$el, function () {
							self.setGhostTextVisible(true);
						});
					}
				});
			});
		}
		else {
			self.$el.slideUp(200, function () {
				self.setVisibility(value);
			});
		}
	},

	knownTags: ['p', 'br', 'ol', 'ul', 'li', 'b', 'i', 's', 'u', 'sup', 'sub', 'code', 'strong', 'em', 'blockquote', 'hr', 'h1', 'h2', 'h3', 'h4', 'h5', 'h6'],

	_keydown: function (e) {
		var self = e.data;
		if (e.which == 9) {
			e.preventDefault();
		}
		else if (e.which === 13 && e.ctrlKey) {
			e.preventDefault();
			self.$textbox.blur();
			self._endEditing(); // this is called because the blur doesn't always invoke a focus loss event
		}
		else if (e.which === 27) {
			e.preventDefault();
			self.$textbox.blur();
			self._endEditing(); // this is called because the blur doesn't always invoke a focus loss event
		}
		else if (e.which === 66 && e.ctrlKey) { //ctrl+b
			document.execCommand('bold', false, null);
		}
		else if (e.which === 73 && e.ctrlKey) { //ctrl+i
			document.execCommand('italic', false, null);
		}
		else if (e.which === 187 && e.ctrlKey) { //ctrl+=
			if (e.shiftKey)
				document.execCommand('superscript', false, null); //ctrl+shift+=
			else
				document.execCommand('subscript', false, null); //ctrl+=
		}
	},

	_mousedown: function (e) {
		var self = e.data;

		self._setEdittable(e.pageX, e.pageY);

		return true;
	},

	_setEdittable: function (pageX, pageY) {

		if (this.editing === true)
			return;

		this.editingSetup = true;

		JASPWidgets.NoteBox.activeNoteBox = this;

		etch.config.selector = '.jasp-editable'

		_.extend(etch.config.buttonClasses, {
			'default': ['bold', 'italic', 'underline'],
			'jasp-comment': ['bold', 'italic', 'superscript', 'subscript', 'unordered-list', 'ordered-list']
		});

		this.editing = true;

		//Only for linux that doesn't have relatedTarget for focusOut event
		if (this.$editor !== undefined)
			this.$editor.off("mousedown", this.editorClicked);
		///////////////////////////////

		this.$editor = etch.startEditing(this.$textbox, pageX, pageY);

		//Only for linux that doesn't have relatedTarget for focusOut event
		this.$editor.on("mousedown", null, this, this.editorClicked);
		///////////////////////////////

		this.$textbox.attr('contenteditable', true);
		this.$textbox.focus();
		// this.updateView();

		this.$el.addClass('jasp-text-editing');

		// this._checkTags();

		var self = this;

		window.setTimeout(function () { self.editingSetup = false; }, 0); //needsd to wait for all ui events to finish before ending
	},

	editorClicked: function (event) {
		var self = event.data;

		self.editorClicked = true;
	},

	_endEditing: function () {
		if (this.editing === false || this.editingSetup === true)
			return;

		this.editing = false;

		if (JASPWidgets.NoteBox.activeNoteBox === this)
			JASPWidgets.NoteBox.activeNoteBox = null;

		this.$el.removeClass('jasp-text-editing');

		// this.updateView();
		this.$textbox.attr('contenteditable', false);
		if (this.$editor !== undefined) {
			this.$editor.off("mousedown", this.editorClicked);
			etch.closeEditor(this.$editor, this.$textbox);
			delete this.$editor;
		}
	},

	_looseFocus: function (e) {
		//Needed to catch the blur event caused while setting up the editing box. This also infers that the the click event was not propagated and needs to be called.
		var self = e.data;
		if (self.editingSetup === true) {
			self.$textbox.click();
			self.$textbox.focus();
			return false;
		}

		var relatedtarget = e.relatedTarget;
		if (relatedtarget === null || $(relatedtarget).not('.etch-editor-panel, .etch-editor-panel *, .etch-image-tools, .etch-image-tools *').size() || (relatedtarget === undefined && !self.editorClicked))
			self._endEditing();

		self.editorClicked = false;

		return true;
	},

	exportBegin: function (exportParams, completedCallback) {

		if (exportParams == undefined)
			exportParams = new JASPWidgets.Exporter.params();
		else if (exportParams.error)
			return false;

		var callback = this.exportComplete;
		if (completedCallback !== undefined)
			callback = completedCallback;

		var html = '';
		if (this.isTextboxEmpty() === false && this.visible === true) {

			html += '<div ' + JASPWidgets.Exporter.getNoteStyles(this.$el, exportParams) + '>' + this.$quill.root.innerHTML + '</div>';
		}

		callback.call(this, exportParams, new JASPWidgets.Exporter.data(null, html));
	},

	exportComplete: function (exportParams, exportContent) {

		if (!exportParams.error)
			pushHTMLToClipboard(exportContent, exportParams);
	},

	useExportNSBF: function() {
		return false;
	},

	onClosed: function() {
		if (this.$textbox !== undefined)
			this.$textbox.off();

		this.closeButton.close();
	}
})


JASPWidgets.Toolbar = JASPWidgets.View.extend({
	initialize: function () {
		$(document).mousedown(this, this._mouseDownGeneral);
		this.fixed = false;
		this.visible = false;
		this.selected = false;
		this.editing = false;
	},

	$title: function() {
		return this.$el.find(this.titleTag);
	},

	setStatus: function(status) {
		this.status = status;
	},

	render: function () {
		this.$el.empty();

		if (this.titleTag !== undefined)
			this.$el.addClass(this.titleTag + "-toolbar");

		if (this.title !== undefined) {
			if (this.titleTag !== undefined)
				this.$el.append('<' + this.titleTag + ' class="in-toolbar toolbar-clickable">' + this.title + '</' + this.titleTag + '>');
			else
				this.$el.append(this.title);
		}

		if (this.hasMenu)
		{
			this.$el.append('<div class="toolbar-button toolbar-clickable jasp-menu jasp-hide"/>')

			//var $menuBtn = this.$el.find(".jasp-menu")

			var $self = this.$el.find(">:first-child");
			$self.tooltip({
				content: "Damo is awesome",
				items: "*",
				disabled: true,
				show: {
					duration: 250,
					effect: "fade",
					delay: 250
				},
				close: function () {
					window.setTimeout(function () {
						$self.tooltip("option", "disabled", true)
					}, 2000)
				},
				position: {
					my: "left top",
					at: "left bottom+5",
					collision: "flipfit"
				}
			})
		}

		if (this.status !== undefined)
			this.$el.append('<div class="status ' + this.status + '"></div')

		return this;
	},

	events: {
		'mousedown .toolbar-clickable': '_mouseDown',
		'focusout': '_looseFocus',
		'keydown .in-toolbar': '_keydown',

	},

	startEdit: function (callbackWhenDone) {
		this.editing	= true;
		var element		= this.$title();

		this["callback"] = callbackWhenDone;

		element.addClass("toolbar-editing");
		element[0].setAttribute("contenteditable", true);

		element.focus();

		element.on("paste", function (event) {
			var pastedData = event.originalEvent.clipboardData.getData('text/plain');
			this.innerHTML = pastedData;
			event.preventDefault();
		});
	},

	endEdit: function (saveTitle) {
		if (this._editEnding)
			return;

		this._editEnding	= true;
		var element			= this.$title();
		element.removeClass("toolbar-editing");

		element[0].setAttribute("contenteditable", false);
		this.editing	= false;
		var selection	= window.getSelection();
		selection.removeAllRanges();

		element.off("paste");

		if (saveTitle)	this.setTitle(element.text());
		else			element.html(this.title);

		this._editEnding = false;

		if(this["callback"] !== undefined && this["callback"] !== null)
		{
			this["callback"](this.title);
			this["callback"] = null;
		}
	},

	setTitle: function(title) {
		this.title = title;
	},

	_looseFocus: function () {
		this.endEdit(true);
	},

	_keydown: function (e) {
		if (!this.editing)
			return;

		if (e.which == 9) {
			e.preventDefault();
		}
		else if (e.which == 13) {
			e.preventDefault();
			this.endEdit(true);
		}
		else if (e.which == 27) {
			e.preventDefault();
			this.endEdit(false);
		}
	},

	_mouseDownGeneral: function (e) {
		if (!e || !e.data) return;
		var self = e.data;
		self.decreaseFixedness();
	},

	setVisibility: function (value) {
		this.visible = value;

		if (this.fixed)
			return;

		if (value) {
			this.$(".toolbar-button").removeClass('jasp-hide');
		}
		else {
			this.$(".toolbar-button").addClass('jasp-hide');
		}
	},

	_mouseDown: function (e) {

		if (this.editing)
			return true;

		if (this.parent.isCollapsed && this.parent.isCollapsed() && this.parent.setCollapsedState) {
			this.parent.setCollapsedState(false);
			return true;
		}

		this.setFixedness(2);

		var $titleLabel = this.$el.find('>:first-child');

		var offset = $titleLabel.offset();

		var posY = offset.top + $titleLabel.height() - $(window).scrollTop() + 3;
		var posX = offset.left - $(window).scrollLeft();
		var posXright = offset.left + $titleLabel.outerWidth() - $(window).scrollLeft();

		this.options.rX = posX;
		this.options.rY = posY;
		this.options.rXright = posXright;

		this.options.x = e.screenX;
		this.options.y = e.screenY;

		if (this.options.hasNotes)
			this.options['noteOptions'] = this.parent.noteOptions();

		if (this.options.hasCollapse)
			this.options['collapseOptions'] = this.parent.collapseOptions();

		this.parent.trigger('toolbar:showMenu', this.parent, this.options);

		return true;
	},

	decreaseFixedness: function() {
		this.setFixedness(this.fixed - 1);
	},

	setFixedness: function (value) {
		this.fixed = Math.max(0, value);
		this.setVisibility(this.visible);
		this.setSelected(this.fixed !== 0);
	},

	setParent: function (parent) {
		this.parent = parent;
		this.options = {
			//If you add something here don't forget to do the same in resultmenuentry.cpp
			hasCopy:				(parent.hasCopy			=== undefined || parent.hasCopy())			&& parent.copyMenuClicked			!== undefined,
			hasCite:				(parent.hasCitation		=== undefined || parent.hasCitation())		&& parent.citeMenuClicked			!== undefined,
			hasNotes:				(parent.hasNotes		=== undefined || parent.hasNotes())			&& parent.notesMenuClicked			!== undefined,
			hasSaveImg:				(parent.isConvertible	=== undefined || parent.isConvertible())	&& parent.saveImageClicked			!== undefined,
			hasEditImg:				(parent.isEditable		=== undefined || parent.isEditable())		&& parent.editImageClicked			!== undefined,
			hasEditTitle:			(parent.hasEditTitle	=== undefined || parent.hasEditTitle())		&& parent.editTitleClicked			!== undefined,
			hasRemove:				(parent.hasRemove		=== undefined || parent.hasRemove())		&& parent.removeMenuClicked			!== undefined,
			hasDuplicate:			(parent.hasDuplicate	=== undefined || parent.hasDuplicate())		&& parent.duplicateMenuClicked		!== undefined,
			hasShowDeps:			(parent.hasShowDeps		=== undefined || parent.hasShowDeps())		&& parent.showDependenciesClicked	!== undefined,
			hasCollapse:			(parent.hasCollapse		=== undefined || parent.hasCollapse())		&& parent.collapseMenuClicked		!== undefined,
			hasLaTeXCode:			(parent.hasLaTeXCode	=== undefined || parent.hasLaTeXCode())		&& parent.latexCodeMenuClicked		!== undefined,
			hasRemoveAllAnalyses:	parent.menuName			=== 'All',
			hasRefreshAllAnalyses:	parent.menuName			=== 'All',

			objectName: parent.menuName,
		};

		this.hasMenu =	this.options.hasCopy		|| this.options.hasCite		|| this.options.hasSaveImg		|| this.options.hasEditImg		||
						this.options.hasDuplicate	|| this.options.hasNotes	|| this.options.hasRemove		|| this.options.hasRemoveAll	||
						this.options.hasEditTitle	|| this.options.hasCollapse || this.options.hasLaTeXCode	|| this.options.hasShowDeps		;
	},

	selectionElement: function() {
		return this.parent.$el;
	},

	setSelected: function (value) {
		this.selected = value;

		var $selectionElement = this.selectionElement();
		if (value) {
			$selectionElement.addClass("jasp-menu-selected")
		}
		else {
			$selectionElement.removeClass("jasp-menu-selected")
		}
	},

	displayMessage: function (msg) {
		if (msg !== undefined) {
			var $self = this.$el.find(">:first-child");
			$self.tooltip("option", "content", msg)
			$self.tooltip("option", "disabled", false)
			$self.tooltip("open")

			window.setTimeout(function () {	$self.tooltip("close")	}, 2000)
		}
	},

	completeEvent: function () {
		this.setFixedness(0);
	}
})

JASPWidgets.Progressbar = Backbone.Model.extend({
	defaults: {
		analysis: {},
		label: "",
		value: -1,
		maxValue: 100
	},

	getFromAnalysis: function(item) {
		return this.attributes.analysis.model.get(item);
	}
});

JASPWidgets.ProgressbarView = JASPWidgets.View.extend({
	initialize: function() {
		this.fadeOutActive = false;
		this.fadeOutDuration = 500;
	},

	render: function() {
		var label = this.model.getFromAnalysis("progress").label
		var value = this.model.getFromAnalysis("progress").value
		if (this._progressbarNeedsToComplete(value)) {
			label = this.model.get("label");
			value = this.model.get("maxValue");
		} else {
			label = this._ellipsify(label);

			var maxValue = this.model.get("maxValue");
			if (value > maxValue)
				value = maxValue;
		}

		this.model.set("value", value);
		this.model.set("label", label);

		if (value == -1 && this.fadeOutActive)
			return this; // allow previously started fade out to complete, avoid calling .clear()

		this.clear();

		if (value != -1) {
			this._insertBar();

			if (this._isComplete()) {
				this._resetModel();
				this._fadeOut();
			}
		}

		return this;
	},

	isActive: function() {
		return this.model.get("value") != -1;
	},

	clear: function() {
		this.$el.empty();
		this.$el.addClass("jasp-progressbar-container");
		this.fadeOutActive = false;
	},

	_isComplete: function() {
		return this.model.get("value") >= this.model.get("maxValue");
	},

	_progressbarNeedsToComplete(value) {
		analysisStatus	= this.model.getFromAnalysis("status");
		return this.isActive() && (value == -1 || analysisStatus == "complete");
	},

	_resetModel: function() {
		var defaults = this.model.defaults;
		defaults.analysis = this.model.get("analysis");
		this.model.clear().set(defaults);
	},

	_fadeOut: function() {
		this.fadeOutActive = true;
		var self = this;
		window.setTimeout(function() {
			self._getCurrent().fadeOut();
			self.fadeOutActive = false;
		}, this.fadeOutDuration);
	},

	_getCurrent: function() {
		return this.$el.find(".jasp-progressbar");
	},

	_insertBar: function() {
		$progressbar = $("<div/>");
		$progressbar.attr({
			class: "jasp-progressbar",
			id: "progressbar-" + this.model.getFromAnalysis("id")
		});

		$bar = $("<progress class=''></progress>");
		$bar.attr({
			value: this.model.get("value"),
			max: this.model.get("maxValue")
		});

		$label = $("<span/>");
		$label.attr({
			class: "jasp-progressbar-label"
		});
		$label.html(this.model.get("label"));

		$progressbar.append($bar);
		$progressbar.append($label);

		this.$el.append($progressbar);
	},

	_ellipsify: function(label) {
		if (label.length == 0)
			return label;

		var alphaNumericOrDotEnding = label.match(/[a-z0-9.]$/i)||[];
		if (alphaNumericOrDotEnding.length == 0)
			return label; // might look weird otherwise, e.g., ~~~...

		var endingDots = label.match(/\.+$/g)||[""];
		var numDotsToAdd = 3 - endingDots[0].length;
		if (numDotsToAdd < 0)
			return label.slice(0, numDotsToAdd);
		return label + ".".repeat(numDotsToAdd);
	}
});

JASPWidgets.ActionView = JASPWidgets.View.extend({
	initialize: function () {
		this.$el.addClass('jasp-hide');

		//$(document).mousemove(this, this._mousemove).mouseup(this, this._mouseup);
	},

	actionDisabled: function () {
		return false;
	},

	actionTargetElement: function () {
		return null;
	},

	setAction: function(action) {
		this.actionCallback = action;
	},

	render: function () {
		this.actionTargetElement().append(this.$el)

		return this;
	},

	events: {
		'click': '_clickHandler'
	},

	_clickHandler: function (e) {
		if (!this.actionDisabled())
			this.actionCallback.call(this);
	},

	setVisibility: function (value) {
		if (value && !this.actionDisabled())
			this.$el.removeClass('jasp-hide');
		else
			this.$el.addClass('jasp-hide');
	}
});

JASPWidgets.Resizeable = Backbone.Model.extend({

	defaults: { width: 150, height: 150 },

	setDim: function (w, h) {
		this.set({ width: w, height: h });
	}
});


JASPWidgets.ResizeableView = JASPWidgets.View.extend({

	/** Property used to silence events from firing when needed. This is not to be set directly. Set using 'resizeStart' method.*/
	silent: false,

	/** Initialises the class by assigning listeners to the model for external size changes and for global mouse events. */
	initialize: function () {
		this._resizeableViewBase = JASPWidgets.View.prototype;

		this.$el.addClass('jasp-hide');

		$(document).mousemove(this, this._mousemove).mouseup(this, this._mouseup);
		this.listenTo(this.model, 'change:width', this.onModelChange);
		this.listenTo(this.model, 'change:height', this.onModelChange);
	},

	/**
	* Returns the DOM tree element whose size attributes with be affected during resizing.
	This allows for the inheriting class to choose another element when required. See image.js
	* @return {Object} DOM Tree element
	*/
	resizeTargetElement: function() {
		return null;
	},

	/**
	* Renders the mouse resize button to the targeted DOM tree element.
	* @return {Object} 'this' for chaining.
	*/
	render: function () {
		this.resizeTargetElement().append(this.$el)

		this._refreshView();
		return this;
	},

	/** Called when an external event changes the views model obejct. */
	onModelChange: function () {
		var size = this._refreshView();
		this.onResized(size.width, size.height);
	},

	/**
	* Called when the model has been resized.
	* @param {Number} w - The width the model was resied to.
	* @param {Number} h - The height the model was resied to.
	*/
	onResized: function (w, h) {
		this.trigger("ResizeableView:resized", w, h)
	},

	/**
	* Refreshes the size of the view using the model information.
	* @return {Object} The size of the view. The object has a width and height property.
	*/
	_refreshView: function(){
		var w = this.model.get('width');
		var h = this.model.get('height');
		this.resizeView(w, h);
		return {
			width: w,
			height: h
		}
	},

	/**
	* Defines the maximum and minimum sizes the view can be resied to.
	* @return {Object} The object has a minWidth, minHeight, maxWidth, maxHeight properties.
	*/
	sizeLimit: function () {
		return {
			minWidth: 160,
			minHeight: 160,
			maxWidth: 2000,
			maxHeight: 2000
		};
	},

	/**
	* Defines the maximum and minimum sizes the view can be resied to.
	* @return {Object} The object has a minWidth, minHeight, maxWidth, maxHeight properties.
	*/
	_mouseResizingStart: function (e) {

		this.iX = e.pageX;
		this.iY = e.pageY;
		var limited = this._normaliseSize(this.model.get("width"), this.model.get("height"));
		this.iW = limited.width;
		this.iH = limited.height;
		this.mouseResizing = true;
		this.resizeStart(this.iW, this.iH, false);
		return false;
	},

	_normaliseSize: function(w, h) {
		var normalised = {};
		var limit = this.sizeLimit();
		normalised.width = Math.min(Math.max(w, limit.minWidth), limit.maxWidth);
		normalised.height = Math.min(Math.max(h, limit.minHeight), limit.maxHeight);
		return normalised;
	},

	resizeStart: function (w, h, silent) {
		this.silent = silent;
		this.resizing = true;
		this.onResizeStart(w, h);
	},

	onResizeStart: function(w, h)
	{
		if (this.silent === false)
			this.trigger("ResizeableView:resizeStart", w, h)
	},

	resizeView: function (w, h) {
		this.resizeTargetElement().css({
			width: w,
			height: h
		});
		this.onResizeView(w, h);
	},

	onResizeView: function (w, h) {
		if (this.silent === false)
			this.trigger("ResizeableView:viewResized", w, h);
	},

	resizeStop: function (w, h) {
		this.model.setDim(w, h);
		this.resizing = false;
		this.mouseResizing = false;
		this.silent = false;
		this.onResizeStop(w, h);
	},

	onResizeStop: function (w, h) {
		if (this.silent === false)
			this.trigger("ResizeableView:resizeStop", w, h)
	},

	isResizing: function () {
		return this.resizing;
	},

	isMouseResizing: function () {
		return this.mouseResizing;
	},

	events: {
			'mousedown': '_mouseResizingStart'
	},

	setVisibility: function (value) {
		if (value)
			this.$el.removeClass('jasp-hide');
		else
			this.$el.addClass('jasp-hide');
	},

	_mouseup: function (e) {
		if (!e || !e.data) return;
		var self = e.data;
		if (self.mouseResizing) {
			var limited = self._normaliseSize(self.iW + e.pageX - self.iX, self.iH + e.pageY - self.iY);
			self.iW = limited.width;
			self.iH = limited.height;
			self.resizeStop.call(self, self.iW, self.iH);
		}
	},

	_mousemove: function (e) {
		if (!e || !e.data) return;
		var self = e.data;
		if (self.mouseResizing) {
			var limited = self._normaliseSize(self.iW + e.pageX - self.iX, self.iH + e.pageY - self.iY);
			self.iW = limited.width;
			self.iH = limited.height;
			self.resizeView(self.iW, self.iH);
			self.iX = e.pageX;
			self.iY = e.pageY;
		}
	},
});
