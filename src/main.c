#include "SVGHelpers.h"
#include "SVGParser.h"

int main(int argc, char **argv) {

    SVG *svg = createSVG(argv[1]);
    // SVG *svg = createValidSVG(argv[1], "svg.xsd");
    if (svg == NULL) {
        return 0;
    }

    bool valid = validateSVG(svg, "svg.xsd");
    if (!valid) {
        printf("Invalid catch\n");
        deleteSVG(svg);
        return 0;
    }

    bool write = writeSVG(svg, "test.svg");
    if (!write) {
        printf("Failed to write\n");
        deleteSVG(svg);
        return 0;
    }

    char *string = SVGToString(svg);
    //printf("%s\n", string);
    free(string);

    char *test;
    List *rects = getRects(svg);

    ListIterator iter = createIterator(rects);
    void *elem;
    while ((elem = nextElement(&iter)) != NULL) {
        Rectangle *rect = (Rectangle *)elem;
        char *tmp = rectToJSON(rect);
        printf("%s\n", tmp);

        free(tmp);
    }

    if (getLength(rects) != 0) {
        //printf("Get Rectanlges -----------\n");
        test = toString(rects);
        //printf("%s\n", test);
        free(test);
    }
    freeList(rects);

    List *circles = getCircles(svg);
    iter = createIterator(circles);
    while((elem = nextElement(&iter))!=NULL) {
        Circle *circ = (Circle *)elem;
        char *tmp = NULL;
        tmp = circleToJSON(circ);
        printf("%s\n",tmp);
        free(tmp);
    }
    if (getLength(circles) != 0) {
        //printf("Get Circles --------\n");
        test = toString(circles);
        //printf("%s\n", test);
        free(test);
    }
    freeList(circles);

    List *paths = getPaths(svg);
    iter = createIterator(paths);
    while ((elem = nextElement(&iter)) != NULL) {
        Path *p = (Path *)elem;
        char *tmp = NULL;
        tmp = pathToJSON(p);
        printf("%s\n", tmp);

        free(tmp);

    }
    if (getLength(paths) != 0) {
        //printf("Get Paths-------\n");
    }
    freeList(paths);

    List *groups = getGroups(svg);
    iter = createIterator(groups);
    while((elem = nextElement(&iter))!=NULL) {
        Group *g = (Group *)elem;
        char *tmp = NULL;
        tmp = groupToJSON(g);
        printf("%s\n",tmp);
        free(tmp);
    }
    freeList(groups);

    char *tmp = NULL;
    tmp = SVGtoJSON(svg);
    printf("%s\n",tmp);
    free(tmp);

    deleteSVG(svg);

    return 0;
}
