# Falcon Config Files

Config files must be placed in the execution path.

## ```fplpath.path```
Paths to search for modules in. Seperated by ```\n```.

## ```fplconfig.json```
JSON object containing config information used by Falcon.

- ```TERMINAL_HAS_COLOR_SUPPORT```
If ```true```, this will enable the printing of error snippets in color at the appropriate locations, or no color if the statement the error occured on spans multiple lines. Otherwise, the error snippets will be printed without color.
