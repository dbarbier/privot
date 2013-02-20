//                                               -*- C++ -*-
/**
 *  @file  GraphImplementation.cxx
 * @brief Graph implements graphic devices for plotting through R
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
 *  @author schueller
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include <cstdlib>
#include <fstream>

#include "GraphImplementation.hxx"
#include "Rfunctions.hxx"
#include "Path.hxx"
#include "PersistentObjectFactory.hxx"
#include "ResourceMap.hxx"
#include "Log.hxx"
#include "OTconfig.hxx"
#include "Os.hxx"

BEGIN_NAMESPACE_OPENTURNS

TEMPLATE_CLASSNAMEINIT(PersistentCollection<Drawable>);

static Factory<PersistentCollection<Drawable> > RegisteredFactory1("PersistentCollection<Drawable>");

CLASSNAMEINIT(GraphImplementation);

static Factory<GraphImplementation> RegisteredFactory("GraphImplementation");

Bool GraphImplementation::IsFirstInitialization = true;


Description GraphImplementation::ValidLegendPositions;

/* Initialize valid legend positions */
void GraphImplementation::initializeValidLegendPositions()
{
  ValidLegendPositions.setName("ValidLegendPositions");
  ValidLegendPositions.add("");
  ValidLegendPositions.add("bottomright");
  ValidLegendPositions.add("bottom");
  ValidLegendPositions.add("bottomleft");
  ValidLegendPositions.add("left");
  ValidLegendPositions.add("topleft");
  ValidLegendPositions.add("top");
  ValidLegendPositions.add("topright");
  ValidLegendPositions.add("right");
  ValidLegendPositions.add("center");
}

/* Default constructor */
GraphImplementation::GraphImplementation(const String & title):
  PersistentObject(),
  title_(title),
  legendPosition_(),
  legendFontSize_(ResourceMap::GetAsNumericalScalar("GraphImplementation-DefaultLegendFontSize")),
  xTitle_(),
  yTitle_(),
  showAxes_(false),
  logScale_(NONE),
  showGrid_(false),
  gridColor_("lightgray"),
  automaticBoundingBox_(true),
  boundingBox_(4),
  drawablesCollection_(0),
  path_(),
  fileName_()
{
  if(IsFirstInitialization)
    {
      initializeValidLegendPositions();
      IsFirstInitialization = false;
    }
}

/* Constructor with parameters */
GraphImplementation::GraphImplementation(const String & title,
                                         const String & xTitle,
                                         const String & yTitle,
                                         const Bool showAxes,
                                         const String & legendPosition,
                                         const NumericalScalar legendFontSize,
                                         const LogScale logScale)
  : PersistentObject(title),
    title_(title),
    legendFontSize_(legendFontSize),
    xTitle_(xTitle),
    yTitle_(yTitle),
    showAxes_(showAxes),
    logScale_(logScale),
    showGrid_(true),
    gridColor_("lightgray"),
    automaticBoundingBox_(true),
    boundingBox_(4),
    drawablesCollection_(0),
    path_(),
    fileName_()
{
  if(IsFirstInitialization)
    {
      initializeValidLegendPositions();
      IsFirstInitialization = false;
    }
  // Check if the given legend position is valid

  if (!isValidLegendPosition(legendPosition)) throw InvalidArgumentException(HERE) << "The given legend position = " << legendPosition << " is invalid";
  if (legendFontSize <= 0.0) throw InvalidArgumentException(HERE) << "The given legend font size = " << legendFontSize << " is invalid, it must be > 0";
  legendPosition_ = legendPosition;
}

/* Virtual constructor */
GraphImplementation * GraphImplementation::clone() const
{
  return new GraphImplementation(*this);
}

/* String converter */
String GraphImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << GraphImplementation::GetClassName()
      << " name=" << getName()
      << " title=" << title_
      << " xTitle=" << xTitle_
      << " yTitle=" << yTitle_
      << " axes=" << (showAxes_ ? "ON" : "OFF")
      << " grid=" << (showGrid_ ? "ON" : "OFF")
      << " legendposition=" << legendPosition_
      << " legendFontSize=" << legendFontSize_
      << " drawables=" << drawablesCollection_
      << " fileName=" << getFileName();
  return oss;
}

/* Adds a drawable instance to the collection of drawables contained in GraphImplementation */
void GraphImplementation::add(const Drawable & aDrawable)
{
  drawablesCollection_.add(aDrawable);
}

/* Erase a drawable instance from the collection of drawables contained in GraphImplementation */
void GraphImplementation::erase(const UnsignedLong i)
{
  drawablesCollection_.erase(drawablesCollection_.begin() + i);
}

/* Adds a collection of drawable instances to the collection of drawables contained in GraphImplementation */
void GraphImplementation::add(const DrawableCollection & drawableCollection)
{
  for (UnsignedLong i = 0; i < drawableCollection.getSize(); ++i) drawablesCollection_.add(drawableCollection[i]);
}

/** Drawables accessor */
GraphImplementation::DrawableCollection GraphImplementation::getDrawables() const
{
  return drawablesCollection_;
}

void GraphImplementation::setDrawables(const DrawableCollection & drawableCollection)
{
  drawablesCollection_ = drawableCollection;
}

/** Individual drawable accessor */
Drawable GraphImplementation::getDrawable(const UnsignedLong index) const
{
  if (index >= drawablesCollection_.getSize()) throw InvalidRangeException(HERE) << "Error: trying to get a drawable at position " << index << " from a collection of size " << drawablesCollection_.getSize();
  return drawablesCollection_[index];
}

void GraphImplementation::setDrawable(const Drawable & drawable,
                                      const UnsignedLong index)
{
  if (index >= drawablesCollection_.getSize()) throw InvalidRangeException(HERE) << "Error: trying to set a drawable at position " << index << " into a collection of size " << drawablesCollection_.getSize();
  drawablesCollection_[index] = drawable;
}

/* Hide or show x and y axes */
void GraphImplementation::setAxes(const Bool showAxes)
{
  showAxes_ = showAxes;
}

/* Accessor for showAxes_ */
Bool GraphImplementation::getAxes() const
{
  return showAxes_;
}

/** Set log scale for x, y both or none axes */
void GraphImplementation::setLogScale(const LogScale logScale)
{
  if (logScale > LOGXY) logScale_ = NONE;
  else logScale_ = logScale;
}

/** Accessor for logScale_ */
GraphImplementation::LogScale GraphImplementation::getLogScale() const
{
  return logScale_;
}

/* Hide or show x grid */
void GraphImplementation::setGrid(const Bool showGrid)
{
  showGrid_ = showGrid;
}

/* Accessor for showGrid_ */
Bool GraphImplementation::getGrid() const
{
  return showGrid_;
}

/* Grid color accessors */
void GraphImplementation::setGridColor(const String & color)
{
  if(!DrawableImplementation::IsValidColor(color)) throw InvalidArgumentException(HERE) << "Given color = " << color << " is incorrect";

  gridColor_ = color;
}

/* Accessor for showGrid_ */
String GraphImplementation::getGridColor() const
{
  return gridColor_;
}

/* Accesor for xTitle */
String GraphImplementation::getXTitle() const
{
  return xTitle_;
}

/* Accessor for xTitle */
void GraphImplementation::setXTitle(const String & title)
{
  xTitle_ = title;
}

/* Accessor for yTitle */
String GraphImplementation::getYTitle() const
{
  return yTitle_;
}

/* Accessor for yTitle */
void GraphImplementation::setYTitle(const String & title)
{
  yTitle_ = title;
}

/* Accesor for title */
String GraphImplementation::getTitle() const
{
  return title_;
}

/* Accesor for title */
void GraphImplementation::setTitle(const String & title)
{
  title_ = title;
}

/* Accessor for path */
String GraphImplementation::getPath() const
{
  return path_;
}

/* Accessor for file name */
String GraphImplementation::getFileName() const
{
  return fileName_;
}

/* Build the R command corresponding to the legend */
String GraphImplementation::makeRLegendCommand() const
{
  OSS labels, colors, lines, points, fill;
  labels << "c(";
  colors << "c(";
  lines << "c(";
  points << "c(";
  fill << "c(";

  Bool drawLegend = false;

  for(DrawableCollection::const_iterator it = drawablesCollection_.begin(); it != drawablesCollection_.end(); ++it)
    {
      if (it->getLegendName().size() != 0)
        {
          drawLegend = true;
          labels << "\"" << it->getLegendName() << "\",";

          if(it->getColor().size() == 0)
            colors << "NA,";
          else colors << "\"" << it->getColor() << "\",";

          if(it->getFillStyle().size() == 0)
            fill << "NA,";
          else fill << "\"" << it->getFillStyle() << "\",";

          if(it->getPointStyle().size() == 0 || it->getFillStyle().size() != 0) //cannot merge fill and point symbol
            points << "NA,";
          else points << it->getPointCode(it->getPointStyle()) << ",";

          if(it->getLineStyle().size() == 0 || it->getFillStyle().size() != 0 ) //cannot merge line and fill symbol
            lines << "NA,";
          else lines << "\"" << it->getLineStyle() << "\",";
        }
    }
  if (drawLegend)
    {
      String labels_str(labels);
      labels_str.replace(labels_str.length() - 1, 1, ")");

      String colors_str(colors);
      colors_str.replace(colors_str.length() - 1, 1, ")");

      String lines_str(lines);
      lines_str.replace(lines_str.length() - 1, 1, ")");

      String points_str(points);
      points_str.replace(points_str.length() - 1, 1, ")");

      String fill_str(fill);
      fill_str.replace(fill_str.length() - 1, 1, ")");

      OSS rCommand;
      rCommand << "legend(\"" << legendPosition_ << "\","
               << "legend=" << labels_str << ","
               << "col=" << colors_str << ","
               << "lty=" << lines_str << ","
               << "pch=" << points_str << ","
               << "fill=" << fill_str << ","
               << "cex=" << legendFontSize_ << ","
               << "bg=\"grey90\",merge=TRUE,density=40)";

      return rCommand;
    }

  else return String();
}

/* Get the R command corresponding to the graph */
String GraphImplementation::getRCommand() const
{
  return OSS() << makeRHeaderCommand() << makeRCoreCommand();
}

/* Make R header commande */
String GraphImplementation::makeRHeaderCommand() const
{
  return OSS() << R_LEGEND << "\n" << "\n" << R_PIE << "\n" << "options(digits=17)" << "\n" << "options(warn=-1)" << "\n";
}

/* Make R core command */
String GraphImplementation::makeRCoreCommand() const
{
  // get the general bounding box
  BoundingBox boundingBox(getBoundingBox());

  //load the R code attached to the general plot
  OSS graphCommand;
  graphCommand << "plot(c(" << boundingBox[0] << "," << boundingBox[1] << "),"
               << "c(" << boundingBox[2] << "," << boundingBox[3] << "),"
               << "type=\"n\",main=\"" << title_ << "\",";
  if (showAxes_)
    {
      graphCommand << "xlab=\"" << xTitle_ << "\",ylab=\"" << yTitle_ << "\","
                   << "axes=TRUE";
    }
  else
    {
      graphCommand << "xlab=\"\",ylab=\"\",axes=FALSE";
    }
  if (showGrid_)
    {
      graphCommand << ", panel.first=grid(col=\"" + gridColor_ + "\")";
    }
  switch (logScale_)
    {
    case NONE:
      break;
    case LOGX:
      graphCommand << ", log=\"x\"";
      break;
    case LOGY:
      graphCommand << ", log=\"y\"";
      break;
    case LOGXY:
      graphCommand << ", log=\"xy\"";
      break;
    default:
      break;
    }
  graphCommand << ", cex.main=2, cex.axis=1.5, cex.lab=1.5)\n";

  // add the R code attached to each drawable
  UnsignedLong drawablesSize(drawablesCollection_.getSize());
  for(UnsignedLong i = 0; i < drawablesSize; ++i)
    {
      if (drawablesCollection_[i].getData().getSize() != 0)
        graphCommand << drawablesCollection_[i].draw() << "\n";
    }
  // make the legend command
  graphCommand << (legendPosition_.size() == 0 ? String() : makeRLegendCommand());
  return graphCommand;
}

/* The method that generates the graphic files */
void GraphImplementation::draw(const String & path,
                               const String & file,
                               const NumericalScalar width,
                               const NumericalScalar height,
                               const Format format)
{
  path_ = path;
  fileName_ = file;
  Format drawingFormat(format);
  // Check the needed drawingFormat. If it is invalid, ste it to ALL
  if ((drawingFormat != ALL) && (drawingFormat != PNG) && (drawingFormat != EPS) && (drawingFormat != FIG) && (drawingFormat != PDF))
    {
      drawingFormat = ALL;
    }
  OSS rCommand;
  rCommand << makeRHeaderCommand();
  String rCoreCommand(makeRCoreCommand());
  if ((drawingFormat == ALL) || (drawingFormat == EPS))
    {
      rCommand << "postscript(\"" << getPostscript() << "\", horizontal = FALSE, onefile = FALSE, paper = \"special\", height=" << height / 72. << ", width=" << width / 72. << ")" << "\n" << rCoreCommand << "\n" << "dev.off()" << "\n";
    }
  if ((drawingFormat == ALL) || (drawingFormat == PDF))
    {
      rCommand << "pdf(\"" << getPDF() << "\", onefile = FALSE, paper = \"special\", height=" << height / 72. << ", width=" << width / 72. << ")" << "\n" << rCoreCommand << "\n" << "dev.off()" << "\n";
    }
  if ((drawingFormat == ALL) || (drawingFormat == PNG))
    {
      rCommand << "png(\"" << getBitmap() << "\",height=" << height << ", width=" << width << ",res=72)" << "\n" << rCoreCommand << "\n" << "dev.off()" << "\n";
    }
  if ((drawingFormat == ALL) || (drawingFormat == FIG))
    {
      rCommand << "xfig(\"" << getVectorial() << "\", horizontal = FALSE, onefile = FALSE, paper = \"A4\", height=" << height / 72. << ", width=" << width / 72. << ")" << "\n" << rCoreCommand << "\n" << "dev.off()" << "\n";
    }

  // system commands to write R code in a temporary file
  /* using mkstemp non standard C for temporary file generation */
  String temporaryFileName(Path::BuildTemporaryFileName("tmp_graph.R.XXXXXX"));
  std::ofstream cmdFile(temporaryFileName.c_str(), std::ios::out);
  cmdFile << String(rCommand);
  cmdFile.close();

  //execute R and load R script in temporary file
  const String RExecutable(ResourceMap::Get("R-executable-command"));
  OSS systemCommand;
  if (RExecutable != "") systemCommand << ResourceMap::Get("R-executable-command") << " --no-save --silent < \"" << temporaryFileName << "\"" << Os::GetDeleteCommandOutput();
  else throw NotYetImplementedException(HERE) << "GraphImplementation::draw() needs R. Please install it and set the absolute path of the R executable in ResourceMap.";
  if (Os::ExecuteCommand(systemCommand) != 0) throw InternalException(HERE) << "GraphImplementation: error trying to execute R command=" << String(systemCommand);
  if (remove(temporaryFileName.c_str()) == -1) LOGWARN(OSS() << "GraphImplementation: error trying to remove file " << temporaryFileName);
  clean();
}

/* Clean temporary files */
void GraphImplementation::clean()
{
  UnsignedLong drawableNumber(drawablesCollection_.getSize());
  // Clean all the temporary data created by the drawables during their drawing
  for (UnsignedLong i = 0; i < drawableNumber; ++i)
    {
      if (drawablesCollection_[i].getData().getSize() != 0)
        drawablesCollection_[i].clean();
    }
}

/* The method that generates the graphic files */
void GraphImplementation::draw(const String & file,
                               const NumericalScalar width,
                               const NumericalScalar height,
                               const Format format)
{
  draw("", file, width, height, format);
}

/* The method returning absolute path of the bitmap graphic file */
String GraphImplementation::getBitmap() const
{
  String bitmap(path_);
  if (path_.size() > 0)
    if ( bitmap.find_last_of(Os::GetDirectorySeparator()) != (bitmap.length() - 1) )
      {
        bitmap += Os::GetDirectorySeparator();
        bitmap += Os::GetDirectorySeparator();
      }
  bitmap += fileName_ + ".png";
  return bitmap;
}

/* The method returning absolute path of the postscript graphic file */
String GraphImplementation::getPostscript() const
{
  String postScript(path_);
  if (path_.size() > 0)
    if ( postScript.find_last_of(Os::GetDirectorySeparator()) != (postScript.length() - 1) )
      {
        postScript += Os::GetDirectorySeparator();
        postScript += Os::GetDirectorySeparator();
      }
  postScript += fileName_ + ".eps";
  return postScript;
}

/* The method returning absolute path of the PDF graphic file */
String GraphImplementation::getPDF() const
{
  String pdf(path_);
  if (path_.size() > 0)
    if ( pdf.find_last_of(Os::GetDirectorySeparator()) != (pdf.length() - 1) )
      {
        pdf += Os::GetDirectorySeparator();
        pdf += Os::GetDirectorySeparator();
      }
  pdf += fileName_ + ".pdf";
  return pdf;
}

/* The method returning absolute path of the vectorial graphic file */
String GraphImplementation::getVectorial() const
{
  String vectorial(path_);
  if (path_.size() > 0)
    if ( vectorial.find_last_of(Os::GetDirectorySeparator()) != (vectorial.length() - 1) )
      {
        vectorial += Os::GetDirectorySeparator();
        vectorial += Os::GetDirectorySeparator();
      }
  vectorial += fileName_ + ".fig";
  return vectorial;
}

/* Get the bounding box of the whole plot */
GraphImplementation::BoundingBox GraphImplementation::getBoundingBox() const
{
  if (automaticBoundingBox_) computeBoundingBox();
  return boundingBox_;
}

/* Set the bounding box of the whole plot */
void GraphImplementation::setBoundingBox(const BoundingBox & boundingBox)
{
  if (boundingBox.getDimension() != 4) throw InvalidArgumentException(HERE) << "Error: the given bounding box must have a dimension equals to 4, here boundingBox=" << boundingBox;
  boundingBox_ = boundingBox;
  automaticBoundingBox_ = false;
}

void GraphImplementation::setBoundingBox(const Interval & boundingBox)
{
  if (boundingBox.getDimension() != 2) throw InvalidArgumentException(HERE) << "Error: the given bounding box must have a dimension equals to 2";
  if (!boundingBox.getFiniteLowerBound()[0])  throw InvalidArgumentException(HERE) << "Error: the lower bound of the first component must be finite";
  if (!boundingBox.getFiniteLowerBound()[1])  throw InvalidArgumentException(HERE) << "Error: the lower bound of the second component must be finite";
  if (!boundingBox.getFiniteUpperBound()[0])  throw InvalidArgumentException(HERE) << "Error: the upper bound of the first component must be finite";
  if (!boundingBox.getFiniteUpperBound()[1])  throw InvalidArgumentException(HERE) << "Error: the upper bound of the second component must be finite";
  boundingBox_[0] = boundingBox.getLowerBound()[0];
  boundingBox_[1] = boundingBox.getUpperBound()[0];
  boundingBox_[2] = boundingBox.getLowerBound()[1];
  boundingBox_[3] = boundingBox.getUpperBound()[1];
  automaticBoundingBox_ = false;
}

/* Automatic bounding box accessor */
Bool GraphImplementation::getAutomaticBoundingBox() const
{
  return automaticBoundingBox_;
}

void GraphImplementation::setAutomaticBoundingBox(const Bool automaticBoundingBox)
{
  automaticBoundingBox_ = automaticBoundingBox;
}

/* Compute the best bounding box to enclose all the drawables */
void GraphImplementation::computeBoundingBox() const
{
  UnsignedLong size(drawablesCollection_.getSize());
  boundingBox_ = BoundingBox(4);
  // First exceptional case: no drawable, we default to default bounding box
  if (size == 0)
    {
      LOGINFO("Warning: cannot compute the bounding box of a graph with no drawable, switch to [0,1]x[0,1] default bounding box");
      boundingBox_[0] = 0.0;
      boundingBox_[1] = 1.0;
      boundingBox_[2] = 0.0;
      boundingBox_[3] = 1.0;
      return;
    }
  NumericalSample boxes(size, 4);

  // first, get each Drawable's bounding box and drawing command
  for(UnsignedLong i = 0; i < size; ++i)
    {
      if (drawablesCollection_[i].getData().getSize() != 0)
        boxes[i] = drawablesCollection_[i].getBoundingBox();
    }

  BoundingBox min(boxes.getMin());
  BoundingBox max(boxes.getMax());
  boundingBox_[0] = min[0];
  boundingBox_[1] = max[1];
  boundingBox_[2] = min[2];
  boundingBox_[3] = max[3];
  // All the bounding boxes are degenerated to a point, we default to a 1x1 box centered at this point
  if ((boundingBox_[0] == boundingBox_[1]) || (boundingBox_[2] == boundingBox_[3]))
    {
      LOGINFO("Warning: the overall bounding box is degenerated to a point. Switch to a 1x1 box centered at this point");
      boundingBox_[0] -= 0.5;
      boundingBox_[1] += 0.5;
      boundingBox_[2] -= 0.5;
      boundingBox_[3] += 0.5;
      return;
    }

  return;
}

/* Get the legend position */
String GraphImplementation::getLegendPosition() const
{
  return legendPosition_;
}

/* Set the legend position */
void GraphImplementation::setLegendPosition(const String & position)
{
  if(!isValidLegendPosition(position)) throw InvalidArgumentException(HERE) << "The given legend position = " << position << " is invalid";

  legendPosition_ = position;
}

/* Gives all the valid legend positions */
Description GraphImplementation::GetValidLegendPositions()
{
  if(IsFirstInitialization)
    {
      initializeValidLegendPositions();
      IsFirstInitialization = false;
    }
  return ValidLegendPositions;
}

/* Get the legend font size */
NumericalScalar GraphImplementation::getLegendFontSize() const
{
  return legendFontSize_;
}

/* Set the legend font size */
void GraphImplementation::setLegendFontSize(const NumericalScalar legendFontSize)
{
  if(legendFontSize <= 0.0) throw InvalidArgumentException(HERE) << "The given legend font size = " << legendFontSize << " is invalid";

  legendFontSize_ = legendFontSize;
}

/* check for legend position validity */
Bool GraphImplementation::isValidLegendPosition(const String & position) const
{
  const Description::const_iterator it = find(ValidLegendPositions.begin(), ValidLegendPositions.end(), position);

  return (it != ValidLegendPositions.end());
}

/* Method save() stores the object through the StorageManager */
void GraphImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "title_", title_ );
  adv.saveAttribute( "legendPosition_", legendPosition_ );
  adv.saveAttribute( "legendFontSize_", legendFontSize_ );
  adv.saveAttribute( "xTitle_", xTitle_ );
  adv.saveAttribute( "yTitle_", yTitle_ );
  adv.saveAttribute( "showAxes_", showAxes_ );
  adv.saveAttribute( "drawablesCollection_", drawablesCollection_ );
  adv.saveAttribute( "path_", path_ );
  adv.saveAttribute( "fileName_", fileName_ );
}

/* Method load() reloads the object from the StorageManager */
void GraphImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "title_", title_ );
  adv.loadAttribute( "legendPosition_", legendPosition_ );
  adv.loadAttribute( "legendFontSize_", legendFontSize_ );
  adv.loadAttribute( "xTitle_", xTitle_ );
  adv.loadAttribute( "yTitle_", yTitle_ );
  adv.loadAttribute( "showAxes_", showAxes_ );
  adv.loadAttribute( "drawablesCollection_", drawablesCollection_ );
  adv.loadAttribute( "path_", path_ );
  adv.loadAttribute( "fileName_", fileName_ );
}



END_NAMESPACE_OPENTURNS
