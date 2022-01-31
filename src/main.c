#include "SVGParser.h"
#include "SVGHelpers.h"

int main(int argc, char **argv) {

    SVG *svg = createSVG(argv[1]);

    SVGToString(svg);
    deleteSVG(svg);

    return 0;
}