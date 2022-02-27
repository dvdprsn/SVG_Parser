#include "SVGParser.h"
#include "SVGHelpers.h"

int main(int argc, char **argv) {

    //SVG *svg = createSVG(argv[1]);
    SVG *svg = createValidSVG(argv[1], "svg.xsd");
    if(svg == NULL){
        printf("Failed to create SVG\n");
        return 0;
    }

    validateSVG(svg, "svg.xsd");
    writeSVG(svg, "test.svg");

    char *string = SVGToString(svg);
    printf("%s\n", string);
    free(string);

    char *test;
    List *rects = getRects(svg);
    if(getLength(rects) != 0) {
        printf("Get Rectanlges -----------\n");
        test = toString(rects);
        printf("%s\n", test);
        free(test);
    }
    freeList(rects);

    
    List *circles = getCircles(svg);
    if(getLength(circles) != 0) {
        printf("Get Circles --------\n");
        test = toString(circles);
        printf("%s\n",test);
        free(test);
    }
    freeList(circles);

    printf("Number of Attributes: %d\n", numAttr(svg));

    deleteSVG(svg);

    return 0;
}
