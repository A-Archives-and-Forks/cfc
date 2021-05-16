### [LIBCIMBAR](https://github.com/sz3/libcimbar)
### [DETAILS](DETAILS.md) | PERFORMANCE | [TODO](TODO.md)

## Numbers of note

* The barcode is `1024x1024` pixels. The individual tiles are `8x8` in a `9x9` grid (there is an empty row/column of spacing on either side)
* **7500** or 8750 bytes per cimbar image, after error correction
	* There are 16 possible symbols per tile, encoding 4 bits
	* There are 4 or 8 possible colors, encoding an additional 2-3 bits per tile.
	* These 6-7 bits per tile work out to a maximum of 9300-10850 bytes per barcode, though in practice this number is reduced by error correction.
	* The default ecc setting is 30/155, which is how we go from 9300 -> 7500 bytes of real data for a 4-color cimbar image.
		* Reed Solomon is not perfect for this use case -- specifically, it corrects byte errors, and cimbar errors tend to involve 1-3 bits at a time. However, since Reed Solomon implementations are ubiquitous, it is currently in use.

## Current sustained benchmark

* 4-color cimbar with ecc=30:
	* 4,717,525 bytes (after compression) in 45s -> 838 kilobits/s (~104 KB/s)

* 8-color cimbar with ecc=30:
	* 4,717,525 bytes in 40s -> 943 kilobits/s (~118 KB/s)

* details:
	* cimbar has built-in compression using zstd. What's being measured here is bits over the wire, e.g. data after compression is applied.
	* these numbers are using https://github.com/sz3/cfc, running with 4 CPU threads on a Qualcomm Snapdragon 625
		* perhaps I will buy a new cell phone to inflate the benchmark numbers.
	* the sender is the cimbar.org wasm implementation. An equivalent command line is `./cimbar_send /path/to/file -s`
		* cimbar.org uses the `shakycam` option to allow the receiver to detect/discard "in between" frames as part of the scan step. This allows it to spend more processing time decoding real data.
	* burst rate can be higher (or lower)
		* to this end, lower ecc settings *can* provide better burst rates
	* 4-color cimbar is currently preferred, and will give more consistent transfer speeds.
	* 8-color cimbar should be considered a prototype within a prototype. It is considerably more sensitive to lighting conditions and color tints.

* other notes:
	* having better lighting in the frame often leads to better results -- this is why cimbar.org has a (mostly) white background. cfc uses android's auto-exposure, auto-focus, etc (it's a very simple app). Good ambient light -- or a white background -- can lead to more consitent quality frame capture.
	* because of the lighting/exposure question, landscape *may* be better than portrait.
	* cfc currently has a low resolution, so the cimbar frame should take up as much of the display as possible (trust the guide brackets)
	* similarly, it's best to keep the camera angle straight-on -- instead of at an angle -- to decode the whole image successfully. Decodes should still happen at higher angles, but the "smaller" part of the image may have more errors than the ECC can deal with.
	* other things to be wary of:
		* glare from light sources.
		* shaky hands.
