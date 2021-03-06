//                                               -*- C++ -*-
/**
 *  @file  Graph.hxx
 *  @brief Graph implements graphic devices for plotting through R
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
 *  Id      Graph.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_GRAPH_HXX
#define OPENTURNS_GRAPH_HXX

#include "TypedInterfaceObject.hxx"
#include "GraphImplementation.hxx"
#include "DrawableImplementation.hxx"
#include "Collection.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class Graph
 *
 * Graph implements graphic devices for plotting through R,
 * and manages drawables to be plotted on the same window
 */

class Graph :
  public TypedInterfaceObject<GraphImplementation>
{

  CLASSNAME;

public:

  typedef Collection<Graph>                             GraphCollection;
  typedef GraphImplementation::BoundingBox              BoundingBox;
  typedef GraphImplementation::DrawableCollection       DrawableCollection;
  typedef GraphImplementation::Format                   Format;
  typedef GraphImplementation::LogScale                 LogScale;

  /** Default constructor */
  Graph(const String & title = "");

  /** Constructor from implementation */
  Graph(const GraphImplementation & implementation);

  /** Constructor with parameters */
  Graph(const String & title,
        const String & xTitle,
        const String & yTitle,
        const Bool showAxes,
        const String & legendPosition = "",
        const NumericalScalar legendFontSize = ResourceMap::GetAsNumericalScalar("GraphImplementation-DefaultLegendFontSize"),
        const LogScale logScale = GraphImplementation::NONE);


  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** Adds a drawable instance to the collection of drawables contained in Graph */
  void add(const Drawable & aDrawable);

  /** Adds a collection of drawable instances to the collection of drawables contained in GraphImplementation */
  void add(const DrawableCollection & drawableCollection);

  /** Erase a drawable instance from the collection of drawables contained in GraphImplementation */
  void erase(const UnsignedLong i);

  /** Drawables accessor */
  DrawableCollection getDrawables() const;
  void setDrawables(const DrawableCollection & drawableCollection);

  /** Individual drawable accessor */
  Drawable getDrawable(const UnsignedLong index) const;
  void setDrawable(const Drawable & drawable,
                   const UnsignedLong index);

  /** Hide or show x and y axes */
  void setAxes(const Bool showAxes);

  /** Accessor for showAxes_ */
  Bool getAxes() const;

  /** Set log scale for x, y both or none axes */
  void setLogScale(const LogScale logScale);

  /** Accessor for logScale_ */
  LogScale getLogScale() const;

  /** Hide or show grid */
  void setGrid(const Bool showGrid);

  /** Accessor for showGrid_ */
  Bool getGrid() const;

  /** Grid color accessors */
  void setGridColor(const String & color);
  String getGridColor() const;

  /** Accesor for xTitle */
  String getXTitle() const;

  /** Accessor for xTitle */
  void setXTitle(const String & title);

  /** Accessor for yTitle */
  String getYTitle() const;

  /** Accessor for yTitle */
  void setYTitle(const String & title);

  /** Accesor for title */
  String getTitle() const;

  /** Accesor for title */
  void setTitle(const String & title);

  /** Accessor for path */
  String getPath() const;

  /** Accessor for file name */
  String getFileName() const;

  /** The method that generates the graphic files */
  void draw(const String & path,
            const String & file,
            const NumericalScalar width,
            const NumericalScalar height,
            const Format format = GraphImplementation::ALL);

  /** The method that generates the graphic files */
  void draw(const String & file,
            const NumericalScalar width = ResourceMap::GetAsUnsignedLong("GraphImplementation-DefaultWidth"),
            const NumericalScalar height = ResourceMap::GetAsUnsignedLong("GraphImplementation-DefaultHeight"),
            const Format format = GraphImplementation::ALL);

  /** Get the R command corresponding to the graph */
  String getRCommand() const;

  /** Clean temporary files */
  void clean();

  /** The method returning absolute path of the bitmap graphic file */
  String getBitmap() const;

  /** The method returning absolute path of the postscript graphic file */
  String getPostscript() const;

  /** The method returning absolute path of the vectorial graphic file */
  String getVectorial() const;

  /** Get the bounding box of the whole plot */
  BoundingBox getBoundingBox() const;

  /** Set the bounding box of the whole plot */
  void setBoundingBox(const BoundingBox & boundingBox);
  void setBoundingBox(const Interval & boundingBox);

  /** Automatic bounding box accessor */
  Bool getAutomaticBoundingBox() const;
  void setAutomaticBoundingBox(const Bool automaticBoundingBox);

  /** Get the legend position */
  String getLegendPosition() const;

  /** Set the legend position */
  void setLegendPosition(const String & position);

  /** Get the legend font size */
  NumericalScalar getLegendFontSize() const;

  /** Set the legend font size */
  void setLegendFontSize(const NumericalScalar legendFontSize);

  /** Check for legend position validity */
  Bool isValidLegendPosition(const String & position) const;

  /** Initialize valid legend positions **/
  void initializeValidLegendPositions();

  /** Gives all the valid legend positions */
  Description getValidLegendPositions();

  /** Gives all the valid legend positions */
  static Description GetValidLegendPositions();

}; /* class Graph */



END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_GRAPH_HXX */
