/**
 * Utility method to format bytes into the most logical magnitude (KB, MB,
 * or GB).
 */
Number.prototype.formatBytes = function()
{
    var units = ['B', 'KB', 'MB', 'GB', 'TB'],
        bytes = this,
        i;

    for (i = 0; bytes >= 1024 && i < 4; i++)
    {
        bytes /= 1024;
    }

    return bytes.toFixed(2) + units[i];
}

function ChunkedUploader(file, options)
{
    if (!this instanceof ChunkedUploader)
    {
        console.log("Uploader create.");
        return new ChunkedUploader(file, options);
    }

    console.log("Uploader.");

    this.file = file;

    this.options = $.extend(
    {
        url: '/upload'
    }, options);

    this.file_size = this.file.size;
    this.chunk_size = 512;
    this.range_start = 0;
    this.range_end = this.chunk_size;

    if ('mozSlice' in this.file)
    {
        this.slice_method = 'mozSlice';
    }
    else if ('webkitSlice' in this.file)
    {
        this.slice_method = 'webkitSlice';
    }
    else
    {
        this.slice_method = 'slice';
    }

    this.upload_request = new XMLHttpRequest();
    this.upload_request.onload = this._onChunkComplete(this);

    // Respond to changes in connection
    if ('onLine' in navigator)
    {
        window.addEventListener('online', this._onConnectionFound);
        window.addEventListener('offline', this._onConnectionLost);
    }
}

ChunkedUploader.prototype =
{

// Internal Methods __________________________________________________

    _upload: function() {
        var self = this,
            chunk;

            console.log("upload called.");

        // Slight timeout needed here (File read / AJAX readystate conflict?)
        setTimeout(function()
        {
            console.log("range_end: " + self.range_end + " file_size: " + self.file_size);
            // Prevent range overflow
            if (self.range_end > self.file_size)
            {
                self.range_end = self.file_size;
            }

            chunk = self.file[self.slice_method](self.range_start, self.range_end);

            self.upload_request.open('PUT', self.options.url, true);
            self.upload_request.overrideMimeType('application/octet-stream');

            if (self.range_start !== 0)
            {
                self.upload_request.setRequestHeader('Content-Range', 'bytes ' + self.range_start + '-' + self.range_end + '/' + self.file_size);
            }

            console.log("sending request");
            self.upload_request.send(chunk);

            // TODO
            // From the looks of things, jQuery expects a string or a map
            // to be assigned to the "data" option. We'll have to use
            // XMLHttpRequest object directly for now...
            /*$.ajax(self.options.url, {
                data: chunk,
                type: 'PUT',
                mimeType: 'application/octet-stream',
                headers: (self.range_start !== 0) ? {
                    'Content-Range': ('bytes ' + self.range_start + '-' + self.range_end + '/' + self.file_size)
                } : {},
                success: self._onChunkComplete
            });*/
        }, 20);
    },

// Event Handlers ____________________________________________________

    _onUploadComplete: function()
    {
        console.log("Upload done!");
    },

    _onChunkComplete: function(self)
    {
        console.log("onChunkComplete called.");

        // If the end range is already the same size as our file, we
        // can assume that our last chunk has been processed and exit
        // out of the function.
        if (self.range_end === self.file_size)
        {
            console.log("range_end === file_size " + self.file_size)
            self._onUploadComplete();
            return;
        }

        // Update our ranges
        self.range_start = self.range_end;
        self.range_end = self.range_start + self.chunk_size;

        // Continue as long as we aren't paused
        if (!self.is_paused)
        {
            self._upload();
        }
    },

    _onConnectionFound: function ()
    {
        this.resume();
    },

    _onConnectionLost: function ()
    {
        this.pause();
    },

// Public Methods ____________________________________________________

    start: function()
    {
        this._upload();
    },

    pause: function()
    {
        this.is_paused = true;
    },

    resume: function()
     {
        this.is_paused = false;
        this._upload();
    }
};
