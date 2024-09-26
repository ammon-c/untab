## untab - Utility for converting tabs to spaces (C++)

### Description

This is a minimal command line utility for converting tabs to
spaces in a text file.  

When I work on software, I strongly prefer editing with spaces
rather than tabs.  So whenever I start to work on some crusty
old source code that's full of tabs, I feed it to **untab**
first.  

**Langauge**:  C++

**Platforms**:  Windows or Linux

---

### Build on Windows

At a Windows command prompt:

* **CD** to the directory containing the **untab** source code.

* Type "**NMAKE -f makefile.windows**" at the command prompt.

If the build is successful, the executable program is written
to **untab.exe** in the same directory.  

---

### Build on Linux

At a shell prompt:

* **cd** to the directory containing the **untab** source code.

* Type "**make -f makefile.linux**" at the command prompt.

If the build is successful, the executable program is written
to a file named **untab** in the same directory.  

---

### Command line usage summary

```
Usage:  untab infile outfile [width]

Where:
    infile   is the name of the file from which to read the
             tabbed input text.  To read from stdin, use a hyphen
             "-" as the filename.
    outfile  is the name of the file to which the untabbed
             output text is written.  To write to stdout, use a
             hyphen "-" as the filename.
    width    indicates the number of characters per tab stop.
             The default is 4.

Note the output cannot be written to the same file from which
the input is read.
```

-*- end -*-

