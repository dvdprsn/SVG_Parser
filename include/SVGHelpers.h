//
// Created by David Pearson on 2022-01-17.
//

#ifndef A1_SVGHELPERS_H
#define A1_SVGHELPERS_H

#include "SVGParser.h"
#include <stdio.h>


void parser(xmlNode *a_node, SVG *svg);

void initSVG(SVG * svgReturn);
void fillSVG(SVG * svg, xmlNode * cur_node);

void createRect(SVG * svg, xmlNode * cur_node);

void initRect(Rectangle *rect);

void createCircle(SVG * svg, xmlNode * cur_node);

void initCircle(Circle *circle);

void createGroup(SVG *svg, xmlNode *cur_node);

void initGroup(Group *group);

Attribute *createAttr(char *name, char value[]);

void createPath(SVG * svg, xmlNode * cur_node);

char *getNS(xmlNode *cur_node);

void setSVGDesc(SVG * svg, xmlNode * cur_node);
void setSVGTitle(SVG * svg, xmlNode * cur_node);

#endif //A1_SVGHELPERS_H
