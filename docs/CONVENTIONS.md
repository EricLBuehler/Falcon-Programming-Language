# FPL Conventions

## Version convention
**MAJOR** . **MINOR** . **PATCH**

Major - Incompatible changes version

Minor - Compatible minor changes version

Patch - Bug fixes

## Curly Braces and Blocks
Left curly is on the first line of the block. The right curly is positioned after the block of code, deindented by 4 spaces

## Indentation
4 Spaces per block

## Private attributes
```_``` ```name```

## Protected attributes
```__``` ```name```

Access using ```_``` ```class name``` ```__``` ```name```

## Sequences
Must define
- ```__getitem__```

Can define
- ```__setitem__```
- ```__delitem__```

## Iterators
Must define
- ```__iter__```

Can define
- ```__next__```