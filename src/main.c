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

    deleteSVG(svg); 
    free(string);
    
    return 0;
}
