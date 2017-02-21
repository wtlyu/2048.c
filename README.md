# 2048.c
CLI version of 2048, written in C.

# Preview
![Demo](demo.gif)

# Build and Install
```
make
make install
```

## For Windows

Please replace the first section in Makefile with this:

```
2048.exe: 2048.c
	cl /nologo 2048.c
```

then run

```
make
```

# Run
```
2048
```

# Uninsall
```
make uninstall
```

# License
Apache License
Version 2.0, January 2004

see LICENSE

# Maintainer
* eastpiger

# Acknowledgement

Special thanks to blakemcbride's project [Drill](https://github.com/blakemcbride/Drill).
