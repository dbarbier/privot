//                                               -*- C++ -*-
/**
 *  @file  GraphImplementation.hxx
 *  @brief GraphImplementation implements graphic devices for plotting through R
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
 *  Id      GraphImplementation.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_GRAPHIMPLEMENTATION_HXX
#define OPENTURNS_GRAPHIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "PersistentCollection.hxx"
#include "Drawable.hxx"
#include "Interval.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class GraphImplementation
 *
 * GraphImplementation implements graphic devices for plotting through R,
 * and manages drawables to be plotted on the same window
 */

class GraphImplementation :
  public PersistentObject
{

  CLASSNAME;

public:

  typedef NumericalPoint                 BoundingBox;
  typedef Collection<Drawable>           DrawableCollection;
  typedef PersistentCollection<Drawable> DrawablePersistentCollection;

  static Description ValidLegendPositions;
  static Bool IsFirstInitialization;

  enum Format { ALL = 0, PNG, EPS, FIG, PDF };
  enum LogScale { NONE = 0, LOGX, LOGY, LOGXY };

  /** Default constructor */
  GraphImplementation(const String & title = "");

  /** Constructor with parameters */
  GraphImplementation(const String & title,
                      const String & xTitle,
                      const String & yTitle,
                      const Bool showAxes,
                      const String & legendPosition = "",
                      const NumericalScalar legendFontSize = ResourceMap::GetAsNumericalScalar("GraphImplementation-DefaultLegendFontSize"),
                      const LogScale logScale = NONE);

  /** Virtual constructor */
  virtual GraphImplementation * clone() const;

  /** String converter */
  String __repr__() const;

  /** Adds a drawable instance to the collection of drawables contained in GraphImplementation */
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

  /** Grid color accessor */
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
            const Format format = ALL);

  /** The method that generates the graphic files */
  void draw(const String & file,
            const NumericalScalar width = ResourceMap::GetAsUnsignedLong("GraphImplementation-DefaultWidth"),
            const NumericalScalar height = ResourceMap::GetAsUnsignedLong("GraphImplementation-DefaultHeight"),
            const Format format = ALL);

  /** Get the R command corresponding to the graph */
  String getRCommand() const;

  /** Clean temporary files */
  void clean();

  /** The method returning absolute path of the bitmap graphic file */
  String getBitmap() const;

  /** The method returning absolute path of the postscript graphic file */
  String getPostscript() const;

  /** The method returning absolute path of the PDF graphic file */
  String getPDF() const;

  /** The method returning absolute path of the vectorial graphic file */
  String getVectorial() const;

  /** Get the bounding box of the whole plot */
  BoundingBox getBoundingBox() const;

  /** Set the bounding box of the whole plot */
  void setBoundingBox(const BoundingBox & boundingBox);

  /** Set the bounding box of the whole plot */
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
  static void initializeValidLegendPositions();

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

  /** Gives all the valid legend positions */
  static Description GetValidLegendPositions();

private :
  /** Compute the best bounding box to enclose all the drawables */
  void computeBoundingBox() const;

  /** Make R legend command */
  String makeRLegendCommand() const;

  /** Make R header command */
  String makeRHeaderCommand() const;

  /** Make R core command */
  String makeRCoreCommand() const;

  /** GraphImplementationic main title */
  String title_;

  /** Legend position */
  String legendPosition_;

  /** Legend font size */
  NumericalScalar legendFontSize_;

  /** x axis title */
  String xTitle_;

  /** y axis title */
  String yTitle_;

  /** if TRUE, displays the axes on the graphic plot. if FALSE, hides the axes */
  Bool showAxes_;

  /** Set the log scale for one, both or non of the axes */
  LogScale logScale_;

  /** if TRUE, displays a grid on the graphic plot. if FALSE, hides the grid */
  Bool showGrid_;

  /** Grid color */
  String gridColor_;

  /** Is the bounding box automatically computed. */
  Bool automaticBoundingBox_;

  /** Current bounding box */
  /* The format is:
   * bb[0] = xmin
   * bb[1] = xmax
   * bb[2] = ymin
   * bb[3] = ymax
   */
  mutable BoundingBox boundingBox_;

  /** The drawables to be plotted */
  DrawablePersistentCollection drawablesCollection_;

  /** Path to latest files generated */
  String path_;

  /** Name of the graphic file*/
  String fileName_;

}; /* class GraphImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_GRAPHIMPLEMENTATION_HXX */
