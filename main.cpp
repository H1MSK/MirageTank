/**
 * @file main.cpp
 * @author H1MSK (ksda47832338@outlook.com)
 * @date 2019-08-14
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "thirdpart/CommandParser/commandParser.hpp"

#include "mainwindow.h"
int main(int argc, char *argv[]) {
    CommandParser *parser = new CommandParser();

    parser->setUsage("Usage: Mirage [options] [-o outputfile] img1 img2");

    parser->addOption(0, "help", "h", "help", "print this help string");
    parser->addOption(1, "output", "o", "output", "specific output file name");

    parser->addOption(0, "gui", "", "gui", "open gui window");

    parser->parse(argc, argv);

    if (parser->checkOption("help")) {
        printf("%s\n", parser->getHelpString());
        return 0;
    }

    if (argc == 1 || parser->checkOption("gui")) {
        int runGUI(int argc, char *argv[]);
        return runGUI(argc, argv);
    }

    const char **container;
    const char *outputFile;
    //    int width = -1, height = -1;

    if (parser->getOption("output", &container)) {
        outputFile = container[0];
    } else {
        outputFile = "out.png";
    }

    if (parser->getOption("", &container) != 2) {
        printf("Error: Syntax incorrect.\n\n%s\n", parser->getHelpString());
        return -1;
    }

    bool produce(const char* file1, const char* file2, const char *outfile);
    if (!produce(container[0], container[1], outputFile)) {
        printf("Error: File produce failed.\n");
        return -1;
    }

    delete parser;
    return 0;
}
