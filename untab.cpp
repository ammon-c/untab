//
// untab.cpp
// A simple C++ program to replace tabs with spaces in a text file.
//
// (C) Copyright 2004,2011 Ammon R. Campbell.
// This code may be distributed freely for non-commercial purposes
// provided it is done so entirely at your own risk.  Commercial
// use prohibited except with the author's express consent.
//
// NOTES:
//  * This program works with plain 8-bit ANSI/ASCII text files.
//    It does not support the various Unicode encodings for text
//    files.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>

#ifndef WIN32
#define strcpy_s(a, b, c) strcpy(a, c)
#define _stricmp strcasecmp
#endif

// Print a short summary of program usage information to the console.
static void usage()
{
    printf(
        "Usage:  untab infile outfile [width]\n"
        "\n"
        "Where:\n"
        "    infile   is the name of the file from which to read the\n"
        "             tabbed input text.  To read from stdin, use a hyphen\n"
        "             \"-\" as the filename.\n"
        "    outfile  is the name of the file to which the untabbed\n"
        "             output text is written.  To write to stdout, use a\n"
        "             hyphen \"-\" as the filename.\n"
        "    width    indicates the number of characters per tab stop.\n"
        "             The default is 4.\n"
        "\n"
        "Note the output cannot be written to the same file from which\n"
        "the input is read.\n"
        );
}

// Reads the next line of text from an open file stream.
// Note that carriage returns and nulls are discarded.
// Returns false if no more text could be read from the file.
static bool read_line(FILE *file, std::string &line)
{
   line.clear();

   if (!file)
      return false;

   // Read characters from the file until a newline is read
   // or the end of file is reached.
   int c = EOF;
   while ((c = fgetc(file)) != EOF && c != '\n')
   {
      // Ignore carriage returns and null characters.
      // Add any other kind of character to the line of text.
      if (c != '\r' && c != '\0')
         line.push_back(static_cast<char>(c));
   }

   return (c != EOF || !line.empty());
}

// In the given text string, replaces tabs with spaces.
// The width parameter indicates the number of spaces per tab stop.
static void untab_line(std::string &line, int width)
{
   std::string out;
   int column = 0;

   for (const auto ch : line)
   {
      if (ch == '\r')
         continue;

      if (ch == '\t')
      {
         // Replace the tab character with enough spaces to
         // reach the next tab stop position.
         do
         {
            ++column;
            out += ' ';
         }
         while ((column % width) != 0);
      }
      else if (ch == '\n')
      {
         column = 0;
         out += ch;
      }
      else
      {
         ++column;
         out += ch;
      }
   }

   line = out;
}

int main(int argc, char **argv)
{
    const char *hyphen = "-";

    // Check if user needs command line help.
    if (argc < 3 || argc > 4)
    {
        usage();
        return EXIT_FAILURE;
    }

    // Get filename arguments from command line.
    char infile[512] = {0};
    char outfile[512] = {0};
    strcpy_s(infile, sizeof(infile), argv[1]);
    strcpy_s(outfile, sizeof(outfile), argv[2]);

    // Prevent overwriting the input file with the output.
    if (_stricmp(infile, outfile) == 0 && _stricmp(infile, hyphen) != 0)
    {
        fprintf(stderr, "ERROR:  The input file and output file cannot be the same file.\n");
        return EXIT_FAILURE;
    }

    // Get the tab width argument, if any.
    int tabwidth = 4;
    if (argc > 3)
    {
        tabwidth = atoi(argv[3]);
        if (tabwidth < 1 || tabwidth > 1000)
        {
            fprintf(stderr, "ERROR:  Invalid tab width '%s'.  Valid range is 1 to 1000.\n", argv[3]);
            return EXIT_FAILURE;
        }
    }

    // Open the input file, unless the user asked for stdin.
    FILE *fin = stdin;
    if (_stricmp(infile, hyphen) != 0)
    {
#ifdef WIN32
        if (fopen_s(&fin, infile, "r") || !fin)
#else
        fin = fopen(infile, "r");
        if (!fin)
#endif
        {
            fprintf(stderr, "ERROR:  Failed opening '%s' for reading.\n", infile);
            return EXIT_FAILURE;
        }
    }

    // Open the output file, unless the user asked for stdout.
    FILE *fout = stdout;
    if (_stricmp(outfile, hyphen) != 0)
    {
#ifdef WIN32
        if (fopen_s(&fout, outfile, "w") || !fout)
#else
        fout = fopen(outfile, "w");
        if (!fout)
#endif
        {
            fprintf(stderr, "ERROR:  Failed opening '%s' for writing.\n", outfile);
            if (_stricmp(infile, hyphen) != 0)
                fclose(fin);
            return EXIT_FAILURE;
        }
    }

    // Process each line of text from the input file, writing
    // the altered text to the output file as we go.
    std::string line;
    while (read_line(fin, line))
    {
        untab_line(line, tabwidth);
        fprintf(fout, "%s\n", line.c_str());
    }

    // Cleanup.
    if (_stricmp(infile, hyphen) != 0)
        fclose(fin);
    if (_stricmp(outfile, hyphen) != 0)
        fclose(fout);

    return EXIT_SUCCESS;
}

