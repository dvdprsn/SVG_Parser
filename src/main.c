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
    // printf("%s\n", string);
    free(string);

    List *rects = getRects(svg);
    char *rectJSON = rectListToJSON(rects);
    printf("%s\n",rectJSON);
    free(rectJSON);
    freeList(rects);

    List *circles = getCircles(svg);
    char *circs = circListToJSON(circles);
    printf("%s\n", circs);
    free(circs);
    freeList(circles);

    List *paths = getPaths(svg);
    char *pathJSON = pathListToJSON(paths);
    printf("%s\n",pathJSON);
    free(pathJSON);
    freeList(paths);

    List *groups = getGroups(svg);
    char *groupJSON = groupListToJSON(groups);
    printf("%s\n", groupJSON);
    free(groupJSON);
    freeList(groups);

    char *svgJSON = SVGtoJSON(svg);
    printf("%s\n", svgJSON);
    free(svgJSON);

    deleteSVG(svg);

    return 0;
}
