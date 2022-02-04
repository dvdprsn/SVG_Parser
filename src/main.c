#include "SVGParser.h"
#include "SVGHelpers.h"

int main(int argc, char **argv) {

    SVG *svg = createSVG(argv[1]);
    if(svg == NULL){
        printf("Failed to create SVG\n");
    }

    //TODO check for failed mallocs and reallcos in code

    char *string = SVGToString(svg);
    printf("%s\n", string);
    free(string);

    printf("Get Rectanlges -----------\n");
    List *rects = getRects(svg);
    char *test = toString(rects);
    printf("%s\n", test);
    free(test);
    freeList(rects);

    printf("Get Circles --------\n");
    List *circles = getCircles(svg);
    test = toString(circles);
    printf("%s\n",test);
    free(test);
    freeList(circles);

    deleteSVG(svg);

    return 0;
}
