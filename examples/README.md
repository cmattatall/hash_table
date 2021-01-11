Source modules that are placed in this (or sub-directories) Will be added
to the examples build tree. Examples are not normally built unless the current
cmake project is also the top-level cmake project. 

The CLI definition for BUILD_EXAMPLES=ON can be provided to 
build tests when this project is included in a larger cmake project.

Ideally, the examples should be a single file executable (with a mainline)
whose purpose is to demonstrate library usage.
