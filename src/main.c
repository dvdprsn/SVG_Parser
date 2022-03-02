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

    List *rects = getRects(svg);

    ListIterator iter = createIterator(rects);
    void *elem;
    while ((elem = nextElement(&iter)) != NULL) {
        Rectangle *rect = (Rectangle *)elem;
        char *attr = attrListToJSON(rect->otherAttributes);
        
        char *tmp = rectToJSON(rect);
        printf("%s\n", tmp);
        printf("%s\n",attr);

        free(attr);

        free(tmp);
    }
    freeList(rects);

    List *circles = getCircles(svg);
    iter = createIterator(circles);
    while((elem = nextElement(&iter))!=NULL) {
        Circle *circ = (Circle *)elem;
        char *attr =attrListToJSON(circ->otherAttributes);
        char *tmp = NULL;
        tmp = circleToJSON(circ);
        printf("%s\n",tmp);
        printf("%s\n",attr);
        free(attr);
        free(tmp);
    }
    freeList(circles);

    List *paths = getPaths(svg);
    iter = createIterator(paths);
    while ((elem = nextElement(&iter)) != NULL) {
        Path *p = (Path *)elem;
        char *tmp = NULL;
        char *attr = attrListToJSON(p->otherAttributes);
        tmp = pathToJSON(p);
        printf("%s\n", tmp);
        printf("%s\n",attr);
        free(attr);
        free(tmp);

    }
    freeList(paths);

    List *groups = getGroups(svg);
    iter = createIterator(groups);
    while((elem = nextElement(&iter))!=NULL) {
        Group *g = (Group *)elem;
        char *attr = attrListToJSON(g->otherAttributes);
        char *tmp = NULL;
        tmp = groupToJSON(g);
        printf("%s\n",tmp);
        printf("%s\n",attr);
        free(attr);
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
