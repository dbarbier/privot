//                                               -*- C++ -*-
/**
 *  @file  DrawableImplementation.cxx
 *  @brief Abstract top-level class for all Drawable
 *
 *  Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author schueller
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include <algorithm>
#include <cstdio>
#include <cstdlib>

#include "DrawableImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "Log.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(DrawableImplementation);

static Factory<DrawableImplementation> RegisteredFactory("DrawableImplementation");

/* default graphic paramaters */

const UnsignedLong DrawableImplementation::BoundingBoxSize  = 4;

Bool DrawableImplementation::IsFirstInitialization          = true;

/* A map  matching keys with R codes for point symbols */
std::map<String, UnsignedLong> DrawableImplementation::SymbolCodes;

/* Accepted colors */
Description DrawableImplementation::ValidColors;

/* Accepted line styles */
Description DrawableImplementation::ValidLineStyles;

/* Accepted fill styles */
Description DrawableImplementation::ValidFillStyles;

void DrawableImplementation::InitializeValidParameterList()
{
  /* A map  matching keys with R codes for point symbols */
  SymbolCodes["square"]       = 0;
  SymbolCodes["circle"]       = 1;
  SymbolCodes["triangleup"]   = 2;
  SymbolCodes["plus"]         = 3;
  SymbolCodes["times"]        = 4;
  SymbolCodes["diamond"]      = 5;
  SymbolCodes["triangledown"] = 6;
  SymbolCodes["star"]         = 8;
  SymbolCodes["fsquare"]      = 15;
  SymbolCodes["fcircle"]      = 16;
  SymbolCodes["ftriangleup"]  = 17;
  SymbolCodes["fdiamond"]     = 18;
  SymbolCodes["bullet"]       = 20;
  SymbolCodes["dot"]          = 127;

  /* Accepted colors */
  ValidColors.setName("ValidColors");
  ValidColors.add("green");
  ValidColors.add("red");
  ValidColors.add("blue");
  ValidColors.add("yellow");
  ValidColors.add("darkblue");
  ValidColors.add("orange");
  ValidColors.add("lightgreen");
  ValidColors.add("darkcyan");
  ValidColors.add("cyan");
  ValidColors.add("magenta");
  ValidColors.add("darkgreen");
  ValidColors.add("violet");
  ValidColors.add("brown");
  ValidColors.add("darkred");
  ValidColors.add("pink");
  ValidColors.add("ivory");
  ValidColors.add("gold");
  ValidColors.add("darkgrey");
  ValidColors.add("grey");
  ValidColors.add("white");
  ValidColors.add("aliceblue");
  ValidColors.add("antiquewhite");
  ValidColors.add("antiquewhite1");
  ValidColors.add("antiquewhite2");
  ValidColors.add("antiquewhite3");
  ValidColors.add("antiquewhite4");
  ValidColors.add("aquamarine");
  ValidColors.add("aquamarine1");
  ValidColors.add("aquamarine2");
  ValidColors.add("aquamarine3");
  ValidColors.add("aquamarine4");
  ValidColors.add("azure");
  ValidColors.add("azure1");
  ValidColors.add("azure2");
  ValidColors.add("azure3");
  ValidColors.add("azure4");
  ValidColors.add("beige");
  ValidColors.add("bisque");
  ValidColors.add("bisque1");
  ValidColors.add("bisque2");
  ValidColors.add("bisque3");
  ValidColors.add("bisque4");
  ValidColors.add("black");
  ValidColors.add("blanchedalmond");
  ValidColors.add("blue1");
  ValidColors.add("blue2");
  ValidColors.add("blue3");
  ValidColors.add("blue4");
  ValidColors.add("blueviolet");
  ValidColors.add("brown1");
  ValidColors.add("brown2");
  ValidColors.add("brown3");
  ValidColors.add("brown4");
  ValidColors.add("burlywood");
  ValidColors.add("burlywood1");
  ValidColors.add("burlywood2");
  ValidColors.add("burlywood3");
  ValidColors.add("burlywood4");
  ValidColors.add("cadetblue");
  ValidColors.add("cadetblue1");
  ValidColors.add("cadetblue2");
  ValidColors.add("cadetblue3");
  ValidColors.add("cadetblue4");
  ValidColors.add("chartreuse");
  ValidColors.add("chartreuse1");
  ValidColors.add("chartreuse2");
  ValidColors.add("chartreuse3");
  ValidColors.add("chartreuse4");
  ValidColors.add("chocolate");
  ValidColors.add("chocolate1");
  ValidColors.add("chocolate2");
  ValidColors.add("chocolate3");
  ValidColors.add("chocolate4");
  ValidColors.add("coral");
  ValidColors.add("coral1");
  ValidColors.add("coral2");
  ValidColors.add("coral3");
  ValidColors.add("coral4");
  ValidColors.add("cornflowerblue");
  ValidColors.add("cornsilk");
  ValidColors.add("cornsilk1");
  ValidColors.add("cornsilk2");
  ValidColors.add("cornsilk3");
  ValidColors.add("cornsilk4");
  ValidColors.add("cyan1");
  ValidColors.add("cyan2");
  ValidColors.add("cyan3");
  ValidColors.add("cyan4");
  ValidColors.add("darkgoldenrod");
  ValidColors.add("darkgoldenrod1");
  ValidColors.add("darkgoldenrod2");
  ValidColors.add("darkgoldenrod3");
  ValidColors.add("darkgoldenrod4");
  ValidColors.add("darkgray");
  ValidColors.add("darkkhaki");
  ValidColors.add("darkmagenta");
  ValidColors.add("darkolivegreen");
  ValidColors.add("darkolivegreen1");
  ValidColors.add("darkolivegreen2");
  ValidColors.add("darkolivegreen3");
  ValidColors.add("darkolivegreen4");
  ValidColors.add("darkorange");
  ValidColors.add("darkorange1");
  ValidColors.add("darkorange2");
  ValidColors.add("darkorange3");
  ValidColors.add("darkorange4");
  ValidColors.add("darkorchid");
  ValidColors.add("darkorchid1");
  ValidColors.add("darkorchid2");
  ValidColors.add("darkorchid3");
  ValidColors.add("darkorchid4");
  ValidColors.add("darksalmon");
  ValidColors.add("darkseagreen");
  ValidColors.add("darkseagreen1");
  ValidColors.add("darkseagreen2");
  ValidColors.add("darkseagreen3");
  ValidColors.add("darkseagreen4");
  ValidColors.add("darkslateblue");
  ValidColors.add("darkslategray");
  ValidColors.add("darkslategray1");
  ValidColors.add("darkslategray2");
  ValidColors.add("darkslategray3");
  ValidColors.add("darkslategray4");
  ValidColors.add("darkslategrey");
  ValidColors.add("darkturquoise");
  ValidColors.add("darkviolet");
  ValidColors.add("deeppink");
  ValidColors.add("deeppink1");
  ValidColors.add("deeppink2");
  ValidColors.add("deeppink3");
  ValidColors.add("deeppink4");
  ValidColors.add("deepskyblue");
  ValidColors.add("deepskyblue1");
  ValidColors.add("deepskyblue2");
  ValidColors.add("deepskyblue3");
  ValidColors.add("deepskyblue4");
  ValidColors.add("dimgray");
  ValidColors.add("dimgrey");
  ValidColors.add("dodgerblue");
  ValidColors.add("dodgerblue1");
  ValidColors.add("dodgerblue2");
  ValidColors.add("dodgerblue3");
  ValidColors.add("dodgerblue4");
  ValidColors.add("firebrick");
  ValidColors.add("firebrick1");
  ValidColors.add("firebrick2");
  ValidColors.add("firebrick3");
  ValidColors.add("firebrick4");
  ValidColors.add("floralwhite");
  ValidColors.add("forestgreen");
  ValidColors.add("gainsboro");
  ValidColors.add("ghostwhite");
  ValidColors.add("gold1");
  ValidColors.add("gold2");
  ValidColors.add("gold3");
  ValidColors.add("gold4");
  ValidColors.add("goldenrod");
  ValidColors.add("goldenrod1");
  ValidColors.add("goldenrod2");
  ValidColors.add("goldenrod3");
  ValidColors.add("goldenrod4");
  ValidColors.add("gray");
  ValidColors.add("gray0");
  ValidColors.add("gray1");
  ValidColors.add("gray2");
  ValidColors.add("gray3");
  ValidColors.add("gray4");
  ValidColors.add("gray5");
  ValidColors.add("gray6");
  ValidColors.add("gray7");
  ValidColors.add("gray8");
  ValidColors.add("gray9");
  ValidColors.add("gray10");
  ValidColors.add("gray11");
  ValidColors.add("gray12");
  ValidColors.add("gray13");
  ValidColors.add("gray14");
  ValidColors.add("gray15");
  ValidColors.add("gray16");
  ValidColors.add("gray17");
  ValidColors.add("gray18");
  ValidColors.add("gray19");
  ValidColors.add("gray20");
  ValidColors.add("gray21");
  ValidColors.add("gray22");
  ValidColors.add("gray23");
  ValidColors.add("gray24");
  ValidColors.add("gray25");
  ValidColors.add("gray26");
  ValidColors.add("gray27");
  ValidColors.add("gray28");
  ValidColors.add("gray29");
  ValidColors.add("gray30");
  ValidColors.add("gray31");
  ValidColors.add("gray32");
  ValidColors.add("gray33");
  ValidColors.add("gray34");
  ValidColors.add("gray35");
  ValidColors.add("gray36");
  ValidColors.add("gray37");
  ValidColors.add("gray38");
  ValidColors.add("gray39");
  ValidColors.add("gray40");
  ValidColors.add("gray41");
  ValidColors.add("gray42");
  ValidColors.add("gray43");
  ValidColors.add("gray44");
  ValidColors.add("gray45");
  ValidColors.add("gray46");
  ValidColors.add("gray47");
  ValidColors.add("gray48");
  ValidColors.add("gray49");
  ValidColors.add("gray50");
  ValidColors.add("gray51");
  ValidColors.add("gray52");
  ValidColors.add("gray53");
  ValidColors.add("gray54");
  ValidColors.add("gray55");
  ValidColors.add("gray56");
  ValidColors.add("gray57");
  ValidColors.add("gray58");
  ValidColors.add("gray59");
  ValidColors.add("gray60");
  ValidColors.add("gray61");
  ValidColors.add("gray62");
  ValidColors.add("gray63");
  ValidColors.add("gray64");
  ValidColors.add("gray65");
  ValidColors.add("gray66");
  ValidColors.add("gray67");
  ValidColors.add("gray68");
  ValidColors.add("gray69");
  ValidColors.add("gray70");
  ValidColors.add("gray71");
  ValidColors.add("gray72");
  ValidColors.add("gray73");
  ValidColors.add("gray74");
  ValidColors.add("gray75");
  ValidColors.add("gray76");
  ValidColors.add("gray77");
  ValidColors.add("gray78");
  ValidColors.add("gray79");
  ValidColors.add("gray80");
  ValidColors.add("gray81");
  ValidColors.add("gray82");
  ValidColors.add("gray83");
  ValidColors.add("gray84");
  ValidColors.add("gray85");
  ValidColors.add("gray86");
  ValidColors.add("gray87");
  ValidColors.add("gray88");
  ValidColors.add("gray89");
  ValidColors.add("gray90");
  ValidColors.add("gray91");
  ValidColors.add("gray92");
  ValidColors.add("gray93");
  ValidColors.add("gray94");
  ValidColors.add("gray95");
  ValidColors.add("gray96");
  ValidColors.add("gray97");
  ValidColors.add("gray98");
  ValidColors.add("gray99");
  ValidColors.add("gray100");
  ValidColors.add("green1");
  ValidColors.add("green2");
  ValidColors.add("green3");
  ValidColors.add("green4");
  ValidColors.add("greenyellow");
  ValidColors.add("grey0");
  ValidColors.add("grey1");
  ValidColors.add("grey2");
  ValidColors.add("grey3");
  ValidColors.add("grey4");
  ValidColors.add("grey5");
  ValidColors.add("grey6");
  ValidColors.add("grey7");
  ValidColors.add("grey8");
  ValidColors.add("grey9");
  ValidColors.add("grey10");
  ValidColors.add("grey11");
  ValidColors.add("grey12");
  ValidColors.add("grey13");
  ValidColors.add("grey14");
  ValidColors.add("grey15");
  ValidColors.add("grey16");
  ValidColors.add("grey17");
  ValidColors.add("grey18");
  ValidColors.add("grey19");
  ValidColors.add("grey20");
  ValidColors.add("grey21");
  ValidColors.add("grey22");
  ValidColors.add("grey23");
  ValidColors.add("grey24");
  ValidColors.add("grey25");
  ValidColors.add("grey26");
  ValidColors.add("grey27");
  ValidColors.add("grey28");
  ValidColors.add("grey29");
  ValidColors.add("grey30");
  ValidColors.add("grey31");
  ValidColors.add("grey32");
  ValidColors.add("grey33");
  ValidColors.add("grey34");
  ValidColors.add("grey35");
  ValidColors.add("grey36");
  ValidColors.add("grey37");
  ValidColors.add("grey38");
  ValidColors.add("grey39");
  ValidColors.add("grey40");
  ValidColors.add("grey41");
  ValidColors.add("grey42");
  ValidColors.add("grey43");
  ValidColors.add("grey44");
  ValidColors.add("grey45");
  ValidColors.add("grey46");
  ValidColors.add("grey47");
  ValidColors.add("grey48");
  ValidColors.add("grey49");
  ValidColors.add("grey50");
  ValidColors.add("grey51");
  ValidColors.add("grey52");
  ValidColors.add("grey53");
  ValidColors.add("grey54");
  ValidColors.add("grey55");
  ValidColors.add("grey56");
  ValidColors.add("grey57");
  ValidColors.add("grey58");
  ValidColors.add("grey59");
  ValidColors.add("grey60");
  ValidColors.add("grey61");
  ValidColors.add("grey62");
  ValidColors.add("grey63");
  ValidColors.add("grey64");
  ValidColors.add("grey65");
  ValidColors.add("grey66");
  ValidColors.add("grey67");
  ValidColors.add("grey68");
  ValidColors.add("grey69");
  ValidColors.add("grey70");
  ValidColors.add("grey71");
  ValidColors.add("grey72");
  ValidColors.add("grey73");
  ValidColors.add("grey74");
  ValidColors.add("grey75");
  ValidColors.add("grey76");
  ValidColors.add("grey77");
  ValidColors.add("grey78");
  ValidColors.add("grey79");
  ValidColors.add("grey80");
  ValidColors.add("grey81");
  ValidColors.add("grey82");
  ValidColors.add("grey83");
  ValidColors.add("grey84");
  ValidColors.add("grey85");
  ValidColors.add("grey86");
  ValidColors.add("grey87");
  ValidColors.add("grey88");
  ValidColors.add("grey89");
  ValidColors.add("grey90");
  ValidColors.add("grey91");
  ValidColors.add("grey92");
  ValidColors.add("grey93");
  ValidColors.add("grey94");
  ValidColors.add("grey95");
  ValidColors.add("grey96");
  ValidColors.add("grey97");
  ValidColors.add("grey98");
  ValidColors.add("grey99");
  ValidColors.add("grey100");
  ValidColors.add("honeydew");
  ValidColors.add("honeydew1");
  ValidColors.add("honeydew2");
  ValidColors.add("honeydew3");
  ValidColors.add("honeydew4");
  ValidColors.add("hotpink");
  ValidColors.add("hotpink1");
  ValidColors.add("hotpink2");
  ValidColors.add("hotpink3");
  ValidColors.add("hotpink4");
  ValidColors.add("indianred");
  ValidColors.add("indianred1");
  ValidColors.add("indianred2");
  ValidColors.add("indianred3");
  ValidColors.add("indianred4");
  ValidColors.add("ivory1");
  ValidColors.add("ivory2");
  ValidColors.add("ivory3");
  ValidColors.add("ivory4");
  ValidColors.add("khaki");
  ValidColors.add("khaki1");
  ValidColors.add("khaki2");
  ValidColors.add("khaki3");
  ValidColors.add("khaki4");
  ValidColors.add("lavender");
  ValidColors.add("lavenderblush");
  ValidColors.add("lavenderblush1");
  ValidColors.add("lavenderblush2");
  ValidColors.add("lavenderblush3");
  ValidColors.add("lavenderblush4");
  ValidColors.add("lawngreen");
  ValidColors.add("lemonchiffon");
  ValidColors.add("lemonchiffon1");
  ValidColors.add("lemonchiffon2");
  ValidColors.add("lemonchiffon3");
  ValidColors.add("lemonchiffon4");
  ValidColors.add("lightblue");
  ValidColors.add("lightblue1");
  ValidColors.add("lightblue2");
  ValidColors.add("lightblue3");
  ValidColors.add("lightblue4");
  ValidColors.add("lightcoral");
  ValidColors.add("lightcyan");
  ValidColors.add("lightcyan1");
  ValidColors.add("lightcyan2");
  ValidColors.add("lightcyan3");
  ValidColors.add("lightcyan4");
  ValidColors.add("lightgoldenrod");
  ValidColors.add("lightgoldenrod1");
  ValidColors.add("lightgoldenrod2");
  ValidColors.add("lightgoldenrod3");
  ValidColors.add("lightgoldenrod4");
  ValidColors.add("lightgoldenrodyellow");
  ValidColors.add("lightgray");
  ValidColors.add("lightgrey");
  ValidColors.add("lightpink");
  ValidColors.add("lightpink1");
  ValidColors.add("lightpink2");
  ValidColors.add("lightpink3");
  ValidColors.add("lightpink4");
  ValidColors.add("lightsalmon");
  ValidColors.add("lightsalmon1");
  ValidColors.add("lightsalmon2");
  ValidColors.add("lightsalmon3");
  ValidColors.add("lightsalmon4");
  ValidColors.add("lightseagreen");
  ValidColors.add("lightskyblue");
  ValidColors.add("lightskyblue1");
  ValidColors.add("lightskyblue2");
  ValidColors.add("lightskyblue3");
  ValidColors.add("lightskyblue4");
  ValidColors.add("lightslateblue");
  ValidColors.add("lightslategray");
  ValidColors.add("lightslategrey");
  ValidColors.add("lightsteelblue");
  ValidColors.add("lightsteelblue1");
  ValidColors.add("lightsteelblue2");
  ValidColors.add("lightsteelblue3");
  ValidColors.add("lightsteelblue4");
  ValidColors.add("lightyellow");
  ValidColors.add("lightyellow1");
  ValidColors.add("lightyellow2");
  ValidColors.add("lightyellow3");
  ValidColors.add("lightyellow4");
  ValidColors.add("limegreen");
  ValidColors.add("linen");
  ValidColors.add("magenta1");
  ValidColors.add("magenta2");
  ValidColors.add("magenta3");
  ValidColors.add("magenta4");
  ValidColors.add("maroon");
  ValidColors.add("maroon1");
  ValidColors.add("maroon2");
  ValidColors.add("maroon3");
  ValidColors.add("maroon4");
  ValidColors.add("mediumaquamarine");
  ValidColors.add("mediumblue");
  ValidColors.add("mediumorchid");
  ValidColors.add("mediumorchid1");
  ValidColors.add("mediumorchid2");
  ValidColors.add("mediumorchid3");
  ValidColors.add("mediumorchid4");
  ValidColors.add("mediumpurple");
  ValidColors.add("mediumpurple1");
  ValidColors.add("mediumpurple2");
  ValidColors.add("mediumpurple3");
  ValidColors.add("mediumpurple4");
  ValidColors.add("mediumseagreen");
  ValidColors.add("mediumslateblue");
  ValidColors.add("mediumspringgreen");
  ValidColors.add("mediumturquoise");
  ValidColors.add("mediumvioletred");
  ValidColors.add("midnightblue");
  ValidColors.add("mintcream");
  ValidColors.add("mistyrose");
  ValidColors.add("mistyrose1");
  ValidColors.add("mistyrose2");
  ValidColors.add("mistyrose3");
  ValidColors.add("mistyrose4");
  ValidColors.add("moccasin");
  ValidColors.add("navajowhite");
  ValidColors.add("navajowhite1");
  ValidColors.add("navajowhite2");
  ValidColors.add("navajowhite3");
  ValidColors.add("navajowhite4");
  ValidColors.add("navy");
  ValidColors.add("navyblue");
  ValidColors.add("oldlace");
  ValidColors.add("olivedrab");
  ValidColors.add("olivedrab1");
  ValidColors.add("olivedrab2");
  ValidColors.add("olivedrab3");
  ValidColors.add("olivedrab4");
  ValidColors.add("orange1");
  ValidColors.add("orange2");
  ValidColors.add("orange3");
  ValidColors.add("orange4");
  ValidColors.add("orangered");
  ValidColors.add("orangered1");
  ValidColors.add("orangered2");
  ValidColors.add("orangered3");
  ValidColors.add("orangered4");
  ValidColors.add("orchid");
  ValidColors.add("orchid1");
  ValidColors.add("orchid2");
  ValidColors.add("orchid3");
  ValidColors.add("orchid4");
  ValidColors.add("palegoldenrod");
  ValidColors.add("palegreen");
  ValidColors.add("palegreen1");
  ValidColors.add("palegreen2");
  ValidColors.add("palegreen3");
  ValidColors.add("palegreen4");
  ValidColors.add("paleturquoise");
  ValidColors.add("paleturquoise1");
  ValidColors.add("paleturquoise2");
  ValidColors.add("paleturquoise3");
  ValidColors.add("paleturquoise4");
  ValidColors.add("palevioletred");
  ValidColors.add("palevioletred1");
  ValidColors.add("palevioletred2");
  ValidColors.add("palevioletred3");
  ValidColors.add("palevioletred4");
  ValidColors.add("papayawhip");
  ValidColors.add("peachpuff");
  ValidColors.add("peachpuff1");
  ValidColors.add("peachpuff2");
  ValidColors.add("peachpuff3");
  ValidColors.add("peachpuff4");
  ValidColors.add("peru");
  ValidColors.add("pink1");
  ValidColors.add("pink2");
  ValidColors.add("pink3");
  ValidColors.add("pink4");
  ValidColors.add("plum");
  ValidColors.add("plum1");
  ValidColors.add("plum2");
  ValidColors.add("plum3");
  ValidColors.add("plum4");
  ValidColors.add("powderblue");
  ValidColors.add("purple");
  ValidColors.add("purple1");
  ValidColors.add("purple2");
  ValidColors.add("purple3");
  ValidColors.add("purple4");
  ValidColors.add("red1");
  ValidColors.add("red2");
  ValidColors.add("red3");
  ValidColors.add("red4");
  ValidColors.add("rosybrown");
  ValidColors.add("rosybrown1");
  ValidColors.add("rosybrown2");
  ValidColors.add("rosybrown3");
  ValidColors.add("rosybrown4");
  ValidColors.add("royalblue");
  ValidColors.add("royalblue1");
  ValidColors.add("royalblue2");
  ValidColors.add("royalblue3");
  ValidColors.add("royalblue4");
  ValidColors.add("saddlebrown");
  ValidColors.add("salmon");
  ValidColors.add("salmon1");
  ValidColors.add("salmon2");
  ValidColors.add("salmon3");
  ValidColors.add("salmon4");
  ValidColors.add("sandybrown");
  ValidColors.add("seagreen");
  ValidColors.add("seagreen1");
  ValidColors.add("seagreen2");
  ValidColors.add("seagreen3");
  ValidColors.add("seagreen4");
  ValidColors.add("seashell");
  ValidColors.add("seashell1");
  ValidColors.add("seashell2");
  ValidColors.add("seashell3");
  ValidColors.add("seashell4");
  ValidColors.add("sienna");
  ValidColors.add("sienna1");
  ValidColors.add("sienna2");
  ValidColors.add("sienna3");
  ValidColors.add("sienna4");
  ValidColors.add("skyblue");
  ValidColors.add("skyblue1");
  ValidColors.add("skyblue2");
  ValidColors.add("skyblue3");
  ValidColors.add("skyblue4");
  ValidColors.add("slateblue");
  ValidColors.add("slateblue1");
  ValidColors.add("slateblue2");
  ValidColors.add("slateblue3");
  ValidColors.add("slateblue4");
  ValidColors.add("slategray");
  ValidColors.add("slategray1");
  ValidColors.add("slategray2");
  ValidColors.add("slategray3");
  ValidColors.add("slategray4");
  ValidColors.add("slategrey");
  ValidColors.add("snow");
  ValidColors.add("snow1");
  ValidColors.add("snow2");
  ValidColors.add("snow3");
  ValidColors.add("snow4");
  ValidColors.add("springgreen");
  ValidColors.add("springgreen1");
  ValidColors.add("springgreen2");
  ValidColors.add("springgreen3");
  ValidColors.add("springgreen4");
  ValidColors.add("steelblue");
  ValidColors.add("steelblue1");
  ValidColors.add("steelblue2");
  ValidColors.add("steelblue3");
  ValidColors.add("steelblue4");
  ValidColors.add("tan");
  ValidColors.add("tan1");
  ValidColors.add("tan2");
  ValidColors.add("tan3");
  ValidColors.add("tan4");
  ValidColors.add("thistle");
  ValidColors.add("thistle1");
  ValidColors.add("thistle2");
  ValidColors.add("thistle3");
  ValidColors.add("thistle4");
  ValidColors.add("tomato");
  ValidColors.add("tomato1");
  ValidColors.add("tomato2");
  ValidColors.add("tomato3");
  ValidColors.add("tomato4");
  ValidColors.add("turquoise");
  ValidColors.add("turquoise1");
  ValidColors.add("turquoise2");
  ValidColors.add("turquoise3");
  ValidColors.add("turquoise4");
  ValidColors.add("violetred");
  ValidColors.add("violetred1");
  ValidColors.add("violetred2");
  ValidColors.add("violetred3");
  ValidColors.add("violetred4");
  ValidColors.add("wheat");
  ValidColors.add("wheat1");
  ValidColors.add("wheat2");
  ValidColors.add("wheat3");
  ValidColors.add("wheat4");
  ValidColors.add("whitesmoke");
  ValidColors.add("yellow1");
  ValidColors.add("yellow2");
  ValidColors.add("yellow3");
  ValidColors.add("yellow4");
  ValidColors.add("yellowgreen");


  /* Accepted line styles */
  ValidLineStyles.setName("ValidLineStyles");
  ValidLineStyles.add("blank");
  ValidLineStyles.add("solid");
  ValidLineStyles.add("dashed");
  ValidLineStyles.add("dotted");
  ValidLineStyles.add("dotdash");
  ValidLineStyles.add("longdash");
  ValidLineStyles.add("twodash");

  /* Accepted fill styles */
  ValidFillStyles.setName("ValidFillStyles");
  ValidFillStyles.add(ResourceMap::Get("DrawableImplementation-DefaultFillStyle"));
  ValidFillStyles.add("shaded");
}

/* Give the colors name */
Description DrawableImplementation::GetValidColors()
{
  if(IsFirstInitialization)
    {
      InitializeValidParameterList();
      IsFirstInitialization = false;
    }
  return ValidColors;
}

/* Give the line style names */
Description DrawableImplementation::GetValidLineStyles()
{
  if(IsFirstInitialization)
    {
      InitializeValidParameterList();
      IsFirstInitialization = false;
    }
  return ValidLineStyles;
}

/* Give the fill style names */
Description DrawableImplementation::GetValidFillStyles()
{
  if(IsFirstInitialization)
    {
      InitializeValidParameterList();
      IsFirstInitialization = false;
    }
  return ValidFillStyles;
}

/* Give the point style names */
Description DrawableImplementation::GetValidPointStyles()
{
  if(IsFirstInitialization)
    {
      InitializeValidParameterList();
      IsFirstInitialization = false;
    }
  Description validPointStyle;
  std::map<String, UnsignedLong>::const_iterator it(SymbolCodes.begin());
  for (it = SymbolCodes.begin(); it != SymbolCodes.end(); ++it)
    {
      validPointStyle.add(it->first);
    }
  return validPointStyle;
}

/* Convert an RGB triplet to a valid hexadecimal code */
String DrawableImplementation::ConvertFromRGB(const UnsignedLong red,
                                              const UnsignedLong green,
                                              const UnsignedLong blue)
{
  return OSS() << "#"
               << std::hex << std::setw(2) << std::setfill('0') << std::right << std::min(red, 255UL)
               << std::hex << std::setw(2) << std::setfill('0') << std::right << std::min(green, 255UL)
               << std::hex << std::setw(2) << std::setfill('0') << std::right << std::min(blue, 255UL);
}

/* Convert an RGBA quadruplet to a valid hexadecimal code */
String DrawableImplementation::ConvertFromRGBA(const UnsignedLong red,
                                               const UnsignedLong green,
                                               const UnsignedLong blue,
                                               const UnsignedLong alpha)
{
  return OSS() << ConvertFromRGB(red, green, blue)
               << std::hex << std::setw(2) << std::setfill('0') << std::right << std::min(alpha, 255UL);
}

/* Convert an RGB triplet to a valid hexadecimal code */
String DrawableImplementation::ConvertFromRGB(const NumericalScalar red,
                                              const NumericalScalar green,
                                              const NumericalScalar blue)
{
  return ConvertFromRGB(static_cast<UnsignedLong>(round(255 * red)),
                        static_cast<UnsignedLong>(round(255 * green)),
                        static_cast<UnsignedLong>(round(255 * blue)));
}

/* Convert an RGBA quadruplet to a valid hexadecimal code */
String DrawableImplementation::ConvertFromRGBA(const NumericalScalar red,
                                               const NumericalScalar green,
                                               const NumericalScalar blue,
                                               const NumericalScalar alpha)
{
  return ConvertFromRGBA(static_cast<UnsignedLong>(round(255 * red)),
                         static_cast<UnsignedLong>(round(255 * green)),
                         static_cast<UnsignedLong>(round(255 * blue)),
                         static_cast<UnsignedLong>(round(255 * alpha)));
}

/* Convert an HSV triplet to a valid hexadecimal code */
NumericalPoint DrawableImplementation::ConvertFromHSVIntoRGB(const NumericalScalar hue,
                                                             const NumericalScalar saturation,
                                                             const NumericalScalar value)
{
  const UnsignedLong i(static_cast<UnsignedLong>(hue / 60.0) % 6);
  const NumericalScalar f(hue / 60.0 - i);
  const NumericalScalar l(value * (1.0 - saturation));
  const NumericalScalar m(value * (1.0 - f * saturation));
  const NumericalScalar n(value * (1.0 - (1.0 - f) * saturation));
  NumericalPoint redGreenBlue(3);
  switch (i)
    {
    case 0:
      redGreenBlue[0] = value;
      redGreenBlue[1] = n;
      redGreenBlue[2] = l;
      break;
    case 1:
      redGreenBlue[0] = m;
      redGreenBlue[1] = value;
      redGreenBlue[2] = l;
      break;
    case 2:
      redGreenBlue[0] = l;
      redGreenBlue[1] = value;
      redGreenBlue[2] = n;
      break;
    case 3:
      redGreenBlue[0] = l;
      redGreenBlue[1] = m;
      redGreenBlue[2] = value;
      break;
    case 4:
      redGreenBlue[0] = n;
      redGreenBlue[1] = l;
      redGreenBlue[2] = value;
      break;
    case 5:
      redGreenBlue[0] = value;
      redGreenBlue[1] = l;
      redGreenBlue[2] = m;
      break;
    default:
      throw InternalException(HERE) << "Error: bad Hue index=" << i << ", should be in {0,...,5}";
    }
  return redGreenBlue;
}

/* Convert an HSV triplet to a valid hexadecimal code */
String DrawableImplementation::ConvertFromHSV(const NumericalScalar hue,
                                              const NumericalScalar saturation,
                                              const NumericalScalar value)
{
  const NumericalPoint redGreenBlue(ConvertFromHSVIntoRGB(hue, saturation, value));
  return ConvertFromRGB(redGreenBlue[0], redGreenBlue[1], redGreenBlue[2]);
}

/* Convert an HSVA quadruplet to a valid hexadecimal code */
String DrawableImplementation::ConvertFromHSVA(const NumericalScalar hue,
                                               const NumericalScalar saturation,
                                               const NumericalScalar value,
                                               const NumericalScalar alpha)
{
  const NumericalPoint redGreenBlue(ConvertFromHSVIntoRGB(hue, saturation, value));
  return ConvertFromRGBA(redGreenBlue[0], redGreenBlue[1], redGreenBlue[2], alpha);
}

/* Default constructor */
DrawableImplementation::DrawableImplementation(const NumericalSample & data,
                                               const String & legend)
  : PersistentObject(legend),
    legendName_(legend),
    data_(data),
    color_(ResourceMap::Get("DrawableImplementation-DefaultColor")),
    fillStyle_(ResourceMap::Get("DrawableImplementation-DefaultFillStyle")),
    lineStyle_(ResourceMap::Get("DrawableImplementation-DefaultLineStyle")),
    pointStyle_(ResourceMap::Get("DrawableImplementation-DefaultPointStyle")),
    lineWidth_(ResourceMap::GetAsUnsignedLong("DrawableImplementation-DefaultLineWidth")),
    dataFileName_("")
{
  if(IsFirstInitialization)
    {
      InitializeValidParameterList();
      IsFirstInitialization = false;
    }
}

/* Clone method */
DrawableImplementation * DrawableImplementation::clone() const
{
  return new DrawableImplementation(*this);
}

/* String converter */
String DrawableImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << DrawableImplementation::GetClassName()
      << " name=" << getName()
      << " legendName=" << legendName_
      << " data=" << data_
      << " color=" << color_
      << " fillStyle=" << fillStyle_
      << " lineStyle=" << lineStyle_
      << " pointStyle=" << pointStyle_
      << " lineWidth=" << lineWidth_;
  return oss;
}

String DrawableImplementation::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName()
      << offset << "(name=" << getName()
      << offset << ", color=" << color_
      << offset << ", fill=" << fillStyle_
      << offset << ", line=" << lineStyle_
      << offset << ", point=" << pointStyle_
      << offset << ", width=" << lineWidth_
      << offset << ", data=\n" << data_.__str__(offset);
  return oss;
}

/* Comparison operator */
Bool DrawableImplementation::operator == (const DrawableImplementation & other) const
{
  return false;
}

/* Legend name accessor */
String DrawableImplementation::getLegendName() const
{
  return legendName_;
}

void DrawableImplementation::setLegendName(const String & legendName)
{
  legendName_ = legendName;
}

/* Point code accessor */
UnsignedLong DrawableImplementation::getPointCode(const String & key) const
{
  const std::map<String, UnsignedLong>::const_iterator it(SymbolCodes.find(key));
  UnsignedLong pointCode = 0;
  if(it != SymbolCodes.end()) pointCode = it->second;

  return pointCode;
}

/* Check validity of color */
Bool DrawableImplementation::IsValidColor(const String & key)
{
  if(IsFirstInitialization)
    {
      InitializeValidParameterList();
      IsFirstInitialization = false;
    }
  // First, check if the color is given in RGB format
  const UnsignedLong keySize(key.size());
  if (keySize == 0) return false;
  // Check if it is a #RRGGBB[AA] code
  if (key[0] == '#')
    {
      // First, check the key length:
      // 7 for #RRGGBB
      // 9 for #RRGGBBAA
      if ((keySize != 7) && (keySize != 9)) return false;
      // Second, check that the values are ok
      for (UnsignedLong i = 1; i < keySize; ++i)
        {
          const char c(key[i]);
          // If the current character is not a valid hexadecimal figure
          const Bool isNum((c >= '0') && (c <= '9'));
          const Bool isValidLower((c >= 'a') && (c <= 'f'));
          const Bool isValidUpper((c >= 'A') && (c <= 'F'));
          if ((!isNum) && !(isValidLower) && !(isValidUpper)) return false;
        }
      return true;
    }
  // Second, check if the color is given by a standard name
  const Description::const_iterator it = std::find(ValidColors.begin(), ValidColors.end(), key);

  return (it != ValidColors.end());
}

/* Check validity of line style */
Bool DrawableImplementation::IsValidLineStyle(const String & key)
{
  if(IsFirstInitialization)
    {
      InitializeValidParameterList();
      IsFirstInitialization = false;
    }
  const Description::const_iterator it = std::find(ValidLineStyles.begin(), ValidLineStyles.end(), key);

  return (it != ValidLineStyles.end());
}

/* Check validity of point style */
Bool DrawableImplementation::IsValidPointStyle(const String & key)
{
  if(IsFirstInitialization)
    {
      InitializeValidParameterList();
      IsFirstInitialization = false;
    }
  const std::map<String, UnsignedLong>::iterator it(SymbolCodes.find(key));

  return (it != SymbolCodes.end());
}

/* Check validity of fill style */
Bool DrawableImplementation::IsValidFillStyle(const String & key)
{
  if(IsFirstInitialization)
    {
      InitializeValidParameterList();
      IsFirstInitialization = false;
    }
  const Description::const_iterator it = std::find(ValidFillStyles.begin(), ValidFillStyles.end(), key);

  return (it != ValidFillStyles.end());
}

/* Check validity of pattern parameter */
Bool DrawableImplementation::IsValidPattern(const String & pattern)
{
  if(IsFirstInitialization)
    {
      InitializeValidParameterList();
      IsFirstInitialization = false;
    }
  return (pattern == ResourceMap::Get("DrawableImplementation-DefaultPattern") || pattern == ResourceMap::Get("DrawableImplementation-AlternativePattern"));
}

/* Check validity of the data */
Bool DrawableImplementation::isValidData(const NumericalSample & data) const
{
  throw NotYetImplementedException(HERE) << "Error: in DrawableImplementation::isValidData";
}

Bool DrawableImplementation::isValidData(const NumericalPoint & data) const
{
  throw NotYetImplementedException(HERE) << "Error: in DrawableImplementation::isValidData";
}

/* Data Accessor */
NumericalSample DrawableImplementation::getData() const
{
  return data_;
}

/* Data accessor */
void DrawableImplementation::setData(const NumericalSample & data)
{
  if(!isValidData(data)) throw InvalidArgumentException(HERE) << "Given data = " << data << " are incorrect";
  data_ = data;
}

void DrawableImplementation::setData(const NumericalPoint & data)
{
  if (!isValidData(data)) throw InvalidArgumentException(HERE) << "Given data = " << data << " are incorrect";
  const UnsignedLong size(data.getDimension());
  data_ = NumericalSample(size, 1);
  for (UnsignedLong i = 0; i < size; ++i) data_[i][0] = data[i];
}

/* Bounding box accessor */
DrawableImplementation::BoundingBox DrawableImplementation::getBoundingBox() const
{
  BoundingBox boundingBox(BoundingBoxSize);
  NumericalPoint min = data_.getMin();
  NumericalPoint max = data_.getMax();
  boundingBox[0] = min[0];
  boundingBox[1] = max[0];
  boundingBox[2] = min[1];
  boundingBox[3] = max[1];
  return boundingBox;
}

/* Accessor for color */
String DrawableImplementation::getColor() const
{
  return color_;
}

/* Accessor for color */
void DrawableImplementation::setColor(const String & color)
{
  if(!IsValidColor(color)) throw InvalidArgumentException(HERE) << "Given color = " << color << " is incorrect";

  color_ = color;
}

/* Accessor for line style */
String DrawableImplementation::getLineStyle() const
{
  return lineStyle_;
}

/* Accesor for line style */
void DrawableImplementation::setLineStyle(const String & lineStyle)
{
  if(!IsValidLineStyle(lineStyle))  throw InvalidArgumentException(HERE) << "Given line style=" << lineStyle << " is incorrect";
  lineStyle_ = lineStyle;
}

/* Accessor for fill style */
String DrawableImplementation::getFillStyle() const
{
  return fillStyle_;
}

/* Accessor for fill style */
void DrawableImplementation::setFillStyle(const String & fillStyle)
{
  if(!IsValidFillStyle(fillStyle)) throw InvalidArgumentException(HERE) << "Given fill style=" << fillStyle << " is incorrect";

  fillStyle_ = fillStyle;
}

/* Accessor for point style */
String DrawableImplementation::getPointStyle() const
{
  return pointStyle_;
}

/* Accessor for point style */
void DrawableImplementation::setPointStyle(const String & pointStyle)
{
  if(!IsValidPointStyle(pointStyle)) throw InvalidArgumentException(HERE) << "Given point symbol=" << pointStyle << " is incorrect";
  pointStyle_ = pointStyle;
}

/* Accessor for line width */
UnsignedLong DrawableImplementation::getLineWidth() const
{
  return lineWidth_;
}

/* Accessor for line width */
void DrawableImplementation::setLineWidth(const UnsignedLong lineWidth)
{
  if(lineWidth == 0) throw InvalidArgumentException(HERE) << "Given line width=" << lineWidth << " is incorrect";
  lineWidth_ = lineWidth;
}

/* Accessor for pattern */
String DrawableImplementation::getPattern() const
{
  throw NotDefinedException(HERE) << "Error: no pattern in " << getClassName();
}

void DrawableImplementation::setPattern(const String style)
{
  throw NotDefinedException(HERE) << "Error: no pattern in " << getClassName();
}

/* Accessor for show points */
Bool DrawableImplementation::getShowPoints() const
{
  throw NotDefinedException(HERE) << "Error: no show points in " << getClassName();
}

void DrawableImplementation::setShowPoints(const Bool showPoints)
{
  throw NotDefinedException(HERE) << "Error: no show points in " << getClassName();
}

/* Accessor for center */
NumericalPoint DrawableImplementation::getCenter() const
{
  throw NotDefinedException(HERE) << "Error: no center in " << getClassName();
}

void DrawableImplementation::setCenter(const NumericalPoint & center)
{
  throw NotDefinedException(HERE) << "Error: no center in " << getClassName();
}

/* Accessor for radius */
NumericalScalar DrawableImplementation::getRadius() const
{
  throw NotDefinedException(HERE) << "Error: no radius in " << getClassName();
}

void DrawableImplementation::setRadius(const NumericalScalar radius)
{
  throw NotDefinedException(HERE) << "Error: no radius in " << getClassName();
}

/* Accessor for labels */
Description DrawableImplementation::getLabels() const
{
  throw NotDefinedException(HERE) << "Error: no labels in " << getClassName();
}

void DrawableImplementation::setLabels(const Description & labels)
{
  throw NotDefinedException(HERE) << "Error: no labels in " << getClassName();
}

/* Accessor for color palette */
Description DrawableImplementation::getPalette() const
{
  throw NotDefinedException(HERE) << "Error: no palette in " << getClassName();
}

void DrawableImplementation::setPalette(const Description & palette)
{
  throw NotDefinedException(HERE) << "Error: no palette in " << getClassName();
}

/* Accessor for origin */
NumericalScalar DrawableImplementation::getOrigin() const
{
  throw NotDefinedException(HERE) << "Error: no origin in " << getClassName();
}

void DrawableImplementation::setOrigin(const NumericalScalar origin)
{
  throw NotDefinedException(HERE) << "Error: no origin in " << getClassName();
}

/* Accessor for first coordinate */
NumericalSample DrawableImplementation::getX() const
{
  throw NotDefinedException(HERE) << "Error: no sample X in " << getClassName();
}

void DrawableImplementation::setX(const NumericalSample & x)
{
  throw NotDefinedException(HERE) << "Error: no sample X in " << getClassName();
}

/* Accessor for second coordinate */
NumericalSample DrawableImplementation::getY() const
{
  throw NotDefinedException(HERE) << "Error: no sample Y in " << getClassName();
}

void DrawableImplementation::setY(const NumericalSample & y)
{
  throw NotDefinedException(HERE) << "Error: no sample Y in " << getClassName();
}

/* Accessor for levels */
NumericalPoint DrawableImplementation::getLevels() const
{
  throw NotDefinedException(HERE) << "Error: no levels in " << getClassName();
}

void DrawableImplementation::setLevels(const NumericalPoint & levels)
{
  throw NotDefinedException(HERE) << "Error: no levels in " << getClassName();
}

/* Accessor for drawLabels */
Bool DrawableImplementation::getDrawLabels() const
{
  throw NotDefinedException(HERE) << "Error: no flag drawLabels in " << getClassName();
}

void DrawableImplementation::setDrawLabels(const Bool & drawLabels)
{
  throw NotDefinedException(HERE) << "Error: no flag drawLabels in " << getClassName();
}

/* R command generating method, for plotting through R */
String DrawableImplementation::draw() const
{
  const UnsignedLong size(data_.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: trying to build a Drawable with empty data";
  // Two strategies: if data is small, it is inlined, else it is passed through a file
  const UnsignedLong dimension(data_.getDimension());
  dataFileName_ = "";
  if (size * dimension > ResourceMap::GetAsUnsignedLong("DrawableImplementation-DataThreshold"))
    {
      dataFileName_ = data_.storeToTemporaryFile();
      return OSS() << "dataOT <- data.matrix(read.table(\"" << dataFileName_ << "\"))";
    }
  return OSS().setPrecision(20) << "dataOT <- " << data_.streamToRFormat() << "\n";
}

/* Clean all the temporary data created by draw() method */
void DrawableImplementation::clean() const
{
  if ((dataFileName_ != "") && (remove(dataFileName_.c_str()) == -1)) LOGWARN(OSS() << "GraphImplementation: error trying to remove file " << dataFileName_);
}

/* Method save() stores the object through the StorageManager */
void DrawableImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "legendName_", legendName_ );
  adv.saveAttribute( "data_", data_ );
  adv.saveAttribute( "color_", color_ );
  adv.saveAttribute( "fillStyle_", fillStyle_ );
  adv.saveAttribute( "lineStyle_", lineStyle_ );
  adv.saveAttribute( "pointStyle_", pointStyle_ );
  adv.saveAttribute( "lineWidth_", lineWidth_ );
}

/* Method load() reloads the object from the StorageManager */
void DrawableImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "legendName_", legendName_ );
  adv.loadAttribute( "data_", data_ );
  adv.loadAttribute( "color_", color_ );
  adv.loadAttribute( "fillStyle_", fillStyle_ );
  adv.loadAttribute( "lineStyle_", lineStyle_ );
  adv.loadAttribute( "pointStyle_", pointStyle_ );
  adv.loadAttribute( "lineWidth_", lineWidth_ );
}



END_NAMESPACE_OPENTURNS
