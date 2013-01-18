//                                               -*- C++ -*-
/**
 *  @file  ResourceMap.cxx
 *  @brief ResourceMap defines top-most resourceMap strategies
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include <unistd.h>                 // for sysconf
#include <string.h>                 // for strdup
#include "OTthread.hxx"
#include "OTconfig.hxx"
#include "OSS.hxx"
#include "ResourceMap.hxx"
#include "Exception.hxx"
//#include "Path.hxx"
#include "Log.hxx"
#include "XMLToolbox.hxx"


BEGIN_NAMESPACE_OPENTURNS



static const char * ConfigurationFileName = "openturns.conf";
static const char * RootElementName = "openturns-configuration";
static const char * XMLTag_value = "value";

static pthread_mutex_t ResourceMap_InstanceMutex_;
static ResourceMap * ResourceMap_P_instance_ = 0;

ResourceMap_init::ResourceMap_init()
{
  static pthread_once_t ResourceMap_InstanceMutex_once = PTHREAD_ONCE_INIT;
  int rc = pthread_once( &ResourceMap_InstanceMutex_once, ResourceMap::Initialize );
  if (rc != 0)
    {
      perror("ResourceMap_init::ResourceMap_init once Initialization failed");
      exit(1);
    }
}

ResourceMap_init::~ResourceMap_init()
{
  ResourceMap::Release();
}



ResourceMapInstance::ResourceMapInstance( ResourceMap & rm )  throw() : rm_(rm), lock_(ResourceMap_InstanceMutex_) {}
ResourceMapInstance::ResourceMapInstance( const ResourceMapInstance & other ) : rm_(other.rm_), lock_(other.lock_) {}
String ResourceMapInstance::__repr__() const
{
  return rm_.__repr__();
}
ResourceMap & ResourceMapInstance::lock() throw()
{
  return rm_;
}
const ResourceMap & ResourceMapInstance::lock() const throw()
{
  return rm_;
}




/* Since ResourceMap is a singleton, GetInstance gives access to the object */
ResourceMapInstance ResourceMap::GetInstance()
{
#ifdef BOGUS_PTHREAD_LIBRARY
  if (!ResourceMap_P_instance_)
    {
      ResourceMap_P_instance_ = new ResourceMap;
      assert(ResourceMap_P_instance_);
    }
#endif
  return *ResourceMap_P_instance_;
}

void ResourceMap::Initialize()
{
  pthread_mutexattr_t attr;
  pthread_mutexattr_init( &attr );
  //pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_NORMAL );
  //pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_ERRORCHECK );
  pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
  int rc = pthread_mutex_init( &ResourceMap_InstanceMutex_, &attr );
  if (rc != 0)
    {
      perror("ResourceMap::Initialize mutex initialization failed");
      exit(1);
    }
#ifdef BOGUS_PTHREAD_LIBRARY
  ResourceMap_P_instance_ = 0;
#else
  ResourceMap_P_instance_ = new ResourceMap;
  assert(ResourceMap_P_instance_);
#endif
}



void ResourceMap::Release()
{
  delete ResourceMap_P_instance_;
  ResourceMap_P_instance_ = 0;
}


/* Get a value in the map */
String ResourceMap::Get(String key)
{
  //MutexLock lock( ResourceMap_InstanceMutex_ );
  return GetInstance().lock().get( key );
}

Bool ResourceMap::GetAsBool(String key)
{
  //MutexLock lock( ResourceMap_InstanceMutex_ );
  return GetInstance().lock().getAsBool( key );
}

UnsignedLong ResourceMap::GetAsUnsignedLong(String key)
{
  //MutexLock lock( ResourceMap_InstanceMutex_ );
  return GetInstance().lock().getAsUnsignedLong( key );
}

NumericalScalar ResourceMap::GetAsNumericalScalar(String key)
{
  //MutexLock lock( ResourceMap_InstanceMutex_ );
  return GetInstance().lock().getAsNumericalScalar( key );
}

char * ResourceMap::GetAsNewCharArray(String key)
{
  //MutexLock lock( ResourceMap_InstanceMutex_ );
  return GetInstance().lock().getAsNewCharArray( key );
}


/* Set a value in the map */
void ResourceMap::Set(String key, String value)
{
  //MutexLock lock( ResourceMap_InstanceMutex_ );
  GetInstance().lock().set( key, value );
}

void ResourceMap::SetAsBool(String key, Bool value)
{
  //MutexLock lock( ResourceMap_InstanceMutex_ );
  GetInstance().lock().setAsBool( key, value );
}

void ResourceMap::SetAsUnsignedLong(String key, UnsignedLong value)
{
  //MutexLock lock( ResourceMap_InstanceMutex_ );
  GetInstance().lock().setAsUnsignedLong( key, value );
}

void ResourceMap::SetAsNumericalScalar(String key, NumericalScalar value)
{
  //MutexLock lock( ResourceMap_InstanceMutex_ );
  GetInstance().lock().setAsNumericalScalar( key, value );
}



/* Default constructor */
ResourceMap::ResourceMap()
  : map_()
{
  loadDefaultConfiguration();
  loadConfigurationFile();
}

/* Method for retrieving information from the resource map */
String ResourceMap::get(String key) const
{
  MapType::const_iterator it = map_.find(key);
  if (it == map_.end()) throw InternalException(HERE) << "Key '" << key << "' is missing in ResourceMap. Report Bug";
  return it->second;
}

Bool ResourceMap::getAsBool(String key) const
{
  Bool value;
  String st = get( key );
  std::istringstream iss( st );
  iss >> value;
  return value;
}

UnsignedLong ResourceMap::getAsUnsignedLong(String key) const
{
  UnsignedLong value;
  String st = get( key );
  std::istringstream iss( st );
  iss >> value;
  return value;
}

NumericalScalar ResourceMap::getAsNumericalScalar(String key) const
{
  NumericalScalar value;
  String st = get( key );
  std::istringstream iss( st );
  iss >> value;
  return value;
}

char * ResourceMap::getAsNewCharArray(String key) const
{
  String st = get( key );
  return strdup( st.c_str() );
}

/* Method for setting information into the resource map */
void ResourceMap::set(String key, String value)
{
  map_[ key ] = value;
}

void ResourceMap::setAsBool(String key, Bool value)
{
  set( key, OSS() << value );
}

void ResourceMap::setAsUnsignedLong(String key, UnsignedLong value)
{
  set( key, OSS() << value );
}

void ResourceMap::setAsNumericalScalar(String key, NumericalScalar value)
{
  set( key, OSS() << value );
}


#if defined HAVE_LIBXML2

/* Update the ResourceMap with information from the configuration file */
void ResourceMap::readConfigurationFile(const FileName & configurationFile)
{
  LOGINFO(OSS() << "Using configuration file " << configurationFile);

  // Open the configuration file and load it in a DOM document for later analysis
  XMLDoc document( configurationFile );

  // Check it is an OpenTURNS' one
  xmlNodePtr rootElt = xmlDocGetRootElement( document );
  if (rootElt == NULL) throw ConfigurationFileParsingException(HERE) << "Wrapper file has no root element" << configurationFile;
  if (xmlStrcmp( rootElt->name, REINTERPRET_CAST(const xmlChar *, RootElementName) ))
    throw ConfigurationFileParsingException(HERE) << "Wrapper file " << configurationFile
                                                  << " has an invalid root element (" << rootElt->name << ")"
                                                  << " at line " << xmlGetLineNo(rootElt);


  // Walk through the DOM document to read the data
  if (rootElt && rootElt->children)
    {
      for (xmlNodePtr current = rootElt->children; current; current = current->next)
        {
          if (XML::IsElement(current))
            {
              String key = XML::ToString(current->name);
              String value = XML::GetAttributeByName( current, XMLTag_value );
              //if (value.empty()) value = XMLAttr_undefined;
              map_[ key ] = value;
            }
        }
    }

}

#else
/* Update the ResourceMap with information from the configuration file */
void ResourceMap::readConfigurationFile(const FileName & configurationFile) {}

#endif

/* Find the configuration file in specific path (see Path class for algorithm) */
FileName ResourceMap::findConfigurationFile() const
{
  Path::DirectoryList dirList = Path::GetConfigDirectoryList();
  return Path::FindFileByNameInDirectoryList(ConfigurationFileName, dirList);
}

/* Load the configuration file */
void ResourceMap::loadConfigurationFile()
{
  try
    {
      readConfigurationFile(findConfigurationFile());
    }
  catch (FileNotFoundException & ex)
    {
      LOGWARN(OSS() << "The configuration file has not been found, using default parameters.");
    }
}



/* Load the configuration defined at installation time */
void ResourceMap::loadDefaultConfiguration()
{
#ifdef R_EXECUTABLE
  set( "R-executable-command", R_EXECUTABLE );
#else
  set( "R-executable-command", "" );
#endif
  set( "csv-file-separator", ";" );
#ifndef WIN32
  set( "temporary-directory", "/tmp" );
  setAsUnsignedLong( "parallel-threads", sysconf(_SC_NPROCESSORS_CONF) );
#else
  set( "temporary-directory", "TEMP" );
  set( "parallel-threads", getenv("NUMBER_OF_PROCESSORS") );
#endif
  set( "user-prefix", "openturnsComputation" );
  set( "real-regexp-shortcut", "\\R" );
  set( "integer-regexp-shortcut", "\\I" );
  set( "separator-regexp-shortcut", "\\S" );
  set( "real-regexp", "([+-]?[ \t]*([0-9]+(\\.[0-9]*)?|\\.[0-9]+)([Ee][+-]?[0-9]+)?)" );
  set( "integer-regexp", "([+-]?[ \t]*[0-9]+)" );
  set( "separator-regexp", "([ \t])" );
  setAsUnsignedLong( "parallel-execution-grainsize", 1 );
  setAsUnsignedLong( "regexp-shortcut-width", 1 );
  setAsUnsignedLong( "cache-max-size", 1024 );
  setAsUnsignedLong( "output-files-timeout", 2 );
  setAsUnsignedLong( "run-command-retries", 3 );
  setAsUnsignedLong( "slow-filesystem-wait-time", 5000 );
  setAsUnsignedLong( "slow-filesystem-error-recovery", 500000 );
  setAsUnsignedLong( "computation-progression-update-interval", 5 );
  setAsBool( "computation-progression-visible", true );

  // OT parameters
  setAsUnsignedLong( "WrapperFile-version", 1 );

  // Collection parameters
  setAsUnsignedLong( "Collection-size-visible-in-str-from", 10 );

  // KFold parameters
  setAsUnsignedLong( "KFold-DefaultK", 10 );

  // BlendedStep parameters //
  setAsNumericalScalar( "BlendedStep-DefaultEta", 0.0 );

  // CenteredFiniteDifferenceGradient parameters //
  setAsNumericalScalar( "CenteredFiniteDifferenceGradient-DefaultEpsilon", 1.0e-5 );

  // CenteredFiniteDifferenceHessian parameters //
  setAsNumericalScalar( "CenteredFiniteDifferenceHessian-DefaultEpsilon", 1.0e-4 );

  // NonCenteredFiniteDifferenceGradient parameters //
  setAsNumericalScalar( "NonCenteredFiniteDifferenceGradient-DefaultEpsilon", 1.0e-7 );

  // SpecFunc parameters //
  setAsNumericalScalar( "SpecFunc-Precision", 1.0e-12 );

  // UniVariatePolynomialImplementation parameters //
  setAsUnsignedLong( "UniVariatePolynomialImplementation-SmallDegree", 400 );

  // Pie parameters //
  setAsNumericalScalar( "Pie-HorizontalMargin", 0.3 );
  setAsNumericalScalar( "Pie-VerticalMargin", 0.1 );
  setAsNumericalScalar( "Pie-LabelThreshold", 0.02 );

  // DrawableImplementation parameters //
  set( "DrawableImplementation-NoSpecifiedLabel", "" );
  set( "DrawableImplementation-DefaultColor", "blue" );
  set( "DrawableImplementation-DefaultSurfaceColor", "white" );
  set( "DrawableImplementation-DefaultFillStyle", "solid" );
  set( "DrawableImplementation-DefaultPointStyle", "plus" );
  set( "DrawableImplementation-DefaultLineStyle", "solid" );
  set( "DrawableImplementation-DefaultPattern", "s" );
  set( "DrawableImplementation-AlternativePattern", "S" );
  setAsUnsignedLong( "DrawableImplementation-DefaultLineWidth", 1 );
  setAsUnsignedLong( "DrawableImplementation-DataThreshold", 2000 );

  // GraphImplementation parameters //
  set( "GraphImplementation-NoSpecifiedLabel", "" );
  setAsUnsignedLong( "GraphImplementation-DefaultWidth", 640 );
  setAsUnsignedLong( "GraphImplementation-DefaultHeight", 480 );
  setAsNumericalScalar( "GraphImplementation-DefaultLegendFontSize", 1.0 );

  // Contour parameters //
  setAsUnsignedLong( "Contour-DefaultLevelsNumber", 10 );

  // BoundConstrainedAlgorithmImplementation parameters //
  setAsUnsignedLong( "BoundConstrainedAlgorithmImplementation-DefaultMaximumEvaluationsNumber", 100 );
  setAsNumericalScalar( "BoundConstrainedAlgorithmImplementation-DefaultMaximumAbsoluteError", 1.0e-5 );
  setAsNumericalScalar( "BoundConstrainedAlgorithmImplementation-DefaultMaximumRelativeError", 1.0e-5 );
  setAsNumericalScalar( "BoundConstrainedAlgorithmImplementation-DefaultMaximumObjectiveError", 1.0e-5 );
  setAsNumericalScalar( "BoundConstrainedAlgorithmImplementation-DefaultMaximumConstraintError", 1.0e-5 );

  // SQP parameters //
  setAsNumericalScalar( "SQP-DefaultTau", 0.5 );
  setAsNumericalScalar( "SQP-DefaultOmega", 1.0e-4 );
  setAsNumericalScalar( "SQP-DefaultSmooth", 1.2 );

  // TNC parameters //
  setAsUnsignedLong( "TNC-DefaultMaxCGit", 50 );
  setAsNumericalScalar( "TNC-DefaultEta", 0.25 );
  setAsNumericalScalar( "TNC-DefaultStepmx", 10.0 );
  setAsNumericalScalar( "TNC-DefaultAccuracy", 1.0e-4 );
  setAsNumericalScalar( "TNC-DefaultFmin", 1.0 );
  setAsNumericalScalar( "TNC-DefaultRescale", 1.3 );

  // AbdoRackwitz parameters //
  setAsNumericalScalar( "AbdoRackwitz-DefaultTau", 0.5 );
  setAsNumericalScalar( "AbdoRackwitz-DefaultOmega", 1.0e-4 );
  setAsNumericalScalar( "AbdoRackwitz-DefaultSmooth", 1.2 );

  // NearestPointAlgorithmImplementation parameters //
  setAsNumericalScalar( "NearestPointAlgorithmImplementation-DefaultLevelValue", 0.0 );
  setAsUnsignedLong( "NearestPointAlgorithmImplementation-DefaultMaximumIterationsNumber", 100 );
  setAsNumericalScalar( "NearestPointAlgorithmImplementation-DefaultMaximumAbsoluteError", 1.0e-5 );
  setAsNumericalScalar( "NearestPointAlgorithmImplementation-DefaultMaximumRelativeError", 1.0e-5 );
  setAsNumericalScalar( "NearestPointAlgorithmImplementation-DefaultMaximumResidualError", 1.0e-5 );
  setAsNumericalScalar( "NearestPointAlgorithmImplementation-DefaultMaximumConstraintError", 1.0e-5 );

  // Cobyla parameters //
  setAsNumericalScalar( "Cobyla-DefaultRhoBeg", 0.1 );

  // SolverImplementation parameters //
  setAsNumericalScalar( "SolverImplementation-DefaultAbsoluteError",  1.0e-5 );
  setAsNumericalScalar( "SolverImplementation-DefaultRelativeError",  1.0e-5 );
  setAsNumericalScalar( "SolverImplementation-DefaultResidualError",  1.0e-8 );
  setAsUnsignedLong( "SolverImplementation-DefaultMaximumFunctionEvaluation", 100 );

  // LinearModelFactory parameters //
  setAsNumericalScalar( "LinearModelFactory-DefaultLevelValue", 0.95 );

  // Last parameters //
  setAsUnsignedLong( "Last-DefaultMaximumSize", 65536 );

  // Compact parameters //
  setAsUnsignedLong( "Compact-DefaultHalfMaximumSize", 1024 );

  // CorrelationAnalysis parameters //
  setAsUnsignedLong( "CorrelationAnalysis-DefaultMaximumOrder", 0 );
  setAsBool( "CorrelationAnalysis-DefaultTotalIndiceComputation", false );

  // FaureSequence parameters //
  setAsUnsignedLong( "FaureSequence-InitialSeed", 1 );

  // HaltonSequence parameters //
  setAsUnsignedLong( "HaltonSequence-InitialSeed", 1 );

  // HaselgroveSequence parameters //
  setAsUnsignedLong( "HaselgroveSequence-InitialSeed", 1 );

  // ReverseHaltonSequence parameters //
  setAsUnsignedLong( "ReverseHaltonSequence-InitialSeed", 1 );

  // SobolSequence parameters //
  setAsUnsignedLong( "SobolSequence-InitialSeed", 1 );

  // SensitivityAnalysis parameters //
  setAsUnsignedLong( "SensitivityAnalysis-DefaultBlockSize", 1 );

  // RandomGenerator parameters //
  setAsUnsignedLong( "RandomGenerator-InitialSeed", 0 );

  // TimeSeries parameters //
  setAsNumericalScalar( "TimeSeries-DefaultEpsilonValue", 1.0e-10 );

  // Collection parameters
  setAsUnsignedLong( "Collection-size-visible-in-str-from", 10 );

  // NumericalSampleImplementation parameters
  setAsUnsignedLong( "NumericalSampleImplementation-SmallKendallTau", 40 );

  // Mesh parameters
  setAsNumericalScalar( "Mesh-SmallVolume", 1.0e-12 );

  // Matrix parameters
  setAsUnsignedLong( "Matrix-size-visible-in-str-from", 5 );

  // Tensor parameters
  setAsUnsignedLong( "Tensor-size-visible-in-str-from", 5 );

  // MatrixImplementation parameters //
  setAsNumericalScalar( "MatrixImplementation-DefaultSmallPivot", 1.0e-7 );

  // BurrFactory parameters //
  setAsNumericalScalar( "BurrFactory-AbsolutePrecision", 1.0e-12 );
  setAsNumericalScalar( "BurrFactory-RelativePrecision", 1.0e-12 );
  setAsNumericalScalar( "BurrFactory-ResidualPrecision", 1.0e-12 );
  setAsUnsignedLong( "BurrFactory-MaximumIteration", 10 );

  // DirichletFactory parameters //
  setAsUnsignedLong( "Dirichlet-DefaultSamplingSize", 500000 );
  setAsUnsignedLong( "Dirichlet-DefaultIntegrationSize", 50 );

  // ConditionalDistribution parameters //
  setAsUnsignedLong( "ConditionalDistribution-MarginalIntegrationNodesNumber", 16 );
  setAsUnsignedLong( "ConditionalDistribution-MaximumIntegrationNodesNumber", 100000 );

  // DirichletFactory parameters //
  setAsNumericalScalar( "DirichletFactory-ParametersEpsilon", 1.0e-12 );
  setAsUnsignedLong( "DirichletFactory-MaximumIteration", 10 );

  // KernelSmoothing parameters //
  setAsUnsignedLong( "KernelSmoothing-SmallSize", 250 );
  setAsNumericalScalar( "KernelSmoothing-CutOffPlugin", 5.0 );
  setAsNumericalScalar( "KernelSmoothing-AbsolutePrecision", 0.0 );
  setAsNumericalScalar( "KernelSmoothing-RelativePrecision", 1.0e-5 );
  setAsNumericalScalar( "KernelSmoothing-ResidualPrecision", 1.0e-10 );
  setAsUnsignedLong( "KernelSmoothing-MaximumIteration", 50 );

  // LogNormal parameters //
  setAsUnsignedLong( "LogNormal-CharacteristicFunctionIntegrationNodes", 256 );
  setAsNumericalScalar( "LogNormal-CharacteristicFunctionSmallSigmaThreshold", 0.2 );

  // LogNormalFactory parameters //
  setAsUnsignedLong( "LogNormalFactory-EstimationMethod", 0 );
  setAsNumericalScalar( "LogNormalFactory-AbsolutePrecision", 1.0e-12 );
  setAsNumericalScalar( "LogNormalFactory-RelativePrecision", 1.0e-12 );
  setAsNumericalScalar( "LogNormalFactory-ResidualPrecision", 1.0e-12 );
  setAsUnsignedLong( "LogNormalFactory-MaximumIteration", 50 );

  // Mixture parameters //
  setAsNumericalScalar( "Mixture-SmallWeight", 1.0e-12 );

  // Multinomial parameters //
  setAsUnsignedLong( "Multinomial-smallA", 10 );
  setAsNumericalScalar( "Multinomial-eta", 1.0e-9 );

  // NegativeBinomialFactory parameters //
  setAsNumericalScalar( "NegativeBinomialFactory-AbsolutePrecision", 1.0e-12 );
  setAsNumericalScalar( "NegativeBinomialFactory-RelativePrecision", 1.0e-12 );
  setAsNumericalScalar( "NegativeBinomialFactory-ResidualPrecision", 1.0e-12 );
  setAsUnsignedLong( "NegativeBinomialFactory-MaximumIteration", 50 );

  // Normal parameters //
  setAsUnsignedLong( "Normal-MaximumNumberOfPoints", 10000000 );
  setAsUnsignedLong( "Normal-MinimumNumberOfPoints", 100000 );
  setAsUnsignedLong( "Normal-SmallDimension", 6 );
  setAsUnsignedLong( "Normal-MarginalIntegrationNodesNumber", 16 );
  setAsNumericalScalar( "Normal-MaximumCDFEpsilon", 5.0e-6 );
  setAsNumericalScalar( "Normal-MinimumCDFEpsilon", 5.0e-2 );

  // RiceFactory parameters //
  setAsNumericalScalar( "RiceFactory-AbsolutePrecision", 1.0e-12 );
  setAsNumericalScalar( "RiceFactory-RelativePrecision", 1.0e-12 );
  setAsNumericalScalar( "RiceFactory-ResidualPrecision", 1.0e-12 );
  setAsUnsignedLong( "RiceFactory-MaximumIteration", 10 );

  // TrapezoidalFactory parameters //
  setAsNumericalScalar( "TrapezoidalFactory-RhoBeg", 0.1 );
  setAsNumericalScalar( "TrapezoidalFactory-RhoEnd", 1.0e-5 );
  setAsUnsignedLong( "TrapezoidalFactory-MaximumIterationsNumber", 10000 );

  // TruncatedDistribution parameters //
  setAsNumericalScalar( "TruncatedDistribution-DefaultThresholdRealization", 0.5 );

  // TruncatedNormalFactory parameters //
  setAsNumericalScalar( "TruncatedNormalFactory-RhoBeg", 0.1 );
  setAsNumericalScalar( "TruncatedNormalFactory-RhoEnd", 1.0e-5 );
  setAsUnsignedLong( "TruncatedNormalFactory-MaximumIterationsNumber", 100000 );

  // Student parameters //
  setAsUnsignedLong( "Student-MaximumNumberOfPoints", 10000000 );
  setAsUnsignedLong( "Student-MinimumNumberOfPoints", 100000 );
  setAsUnsignedLong( "Student-SmallDimension", 6 );
  setAsUnsignedLong( "Student-MarginalIntegrationNodesNumber", 16 );
  setAsNumericalScalar( "Student-MaximumCDFEpsilon", 5.0e-6 );
  setAsNumericalScalar( "Student-MinimumCDFEpsilon", 5.0e-2 );

  // AliMikhailHaqCopulaFactory parameters //
  setAsNumericalScalar( "AliMikhailHaqCopulaFactory-ThetaEpsilon", 1.0e-14 );

  // FrankCopulaFactory parameters //
  setAsNumericalScalar( "FrankCopulaFactory-ThetaEpsilon", 1.0e-14 );

  // RandomMixture parameters //
  setAsUnsignedLong( "RandomMixture-DefaultBlockMin", 3 );
  setAsUnsignedLong( "RandomMixture-DefaultBlockMax", 16 );
  setAsUnsignedLong( "RandomMixture-DefaultMaxSize", 65536 );
  setAsNumericalScalar( "RandomMixture-DefaultAlpha", 5.0 );
  setAsNumericalScalar( "RandomMixture-DefaultPDFEpsilon", 1.0e-10 );
  setAsNumericalScalar( "RandomMixture-DefaultCDFEpsilon", 1.0e-10 );
  setAsNumericalScalar( "RandomMixture-GraphPDFEpsilon", 1.0e-5 );
  setAsNumericalScalar( "RandomMixture-GraphCDFEpsilon", 1.0e-5 );
  setAsUnsignedLong( "RandomMixture-SmallSize", 100 );
  setAsUnsignedLong( "RandomMixture-ProjectionDefaultSize", 10000 );
  setAsUnsignedLong( "RandomMixture-ProjectionMaximumIteration", 1000);
  setAsNumericalScalar( "RandomMixture-ProjectionMaximumAbsoluteError", 1.0e-7 );
  setAsNumericalScalar( "RandomMixture-ProjectionMaximumRelativeError", 1.0e-7 );
  setAsNumericalScalar( "RandomMixture-ProjectionMaximumObjectiveError", 1.0e-7 );
  setAsNumericalScalar( "RandomMixture-ProjectionMaximumConstraintError", 1.0e-7 );

  // NumericalMathEvaluation parameters //
  setAsUnsignedLong( "NumericalMathEvaluationImplementation-DefaultPointNumber", 129 );

  // DistFunc parameters //
  setAsNumericalScalar( "DistFunc-Precision", 1.0e-14 );
  setAsUnsignedLong( "DistFunc-MaximumIteration", 5000 );

  // RootStrategyImplementation parameters //
  setAsNumericalScalar( "RootStrategyImplementation-DefaultMaximumDistance", 8.0 );
  setAsNumericalScalar( "RootStrategyImplementation-DefaultStepSize", 1.0 );

  // Simulation parameters //
  setAsUnsignedLong( "Simulation-DefaultMaximumOuterSampling", 1000 );
  setAsNumericalScalar( "Simulation-DefaultMaximumCoefficientOfVariation", 1.0e-1 );
  setAsNumericalScalar( "Simulation-DefaultMaximumStandardDeviation", 0.0 );
  setAsUnsignedLong( "Simulation-DefaultBlockSize", 1 );

  // SimulationResultImplementation parameters //
  setAsNumericalScalar( "SimulationResultImplementation-DefaultConfidenceLevel", 0.95 );

  // SimulationSensitivityAnalysis parameters //
  setAsUnsignedLong( "SimulationSensitivityAnalysis-DefaultSampleMargin", 400 );

  // AnalyticalResult parameters //
  setAsNumericalScalar( "AnalyticalResult-DefaultWidth", 1.0 );

  // StrongMaximumTest parameters //
  setAsNumericalScalar( "StrongMaximumTest-DefaultDeltaPrecision", 1.0e-7 );
  setAsNumericalScalar( "StrongMaximumTest-Epsilon", 1.0e-10 );

  // CleaningStrategy parameters //
  setAsUnsignedLong( "CleaningStrategy-DefaultMaximumSize", 20 );
  setAsNumericalScalar( "CleaningStrategy-DefaultSignificanceFactor", 1.0e-4 );

  // FunctionalChaosAlgorithm parameters //
  setAsNumericalScalar( "FunctionalChaosAlgorithm-DefaultMaximumResidual", 1.0e-6 );

  // WeightedExperiment parameters //
  setAsUnsignedLong( "WeightedExperiment-DefaultSize", 100 );

  // GaussProductExperiment parameters //
  setAsUnsignedLong( "GaussProductExperiment-DefaultMarginalDegree", 5 );

  // HyperbolicAnisotropicEnumerateFunction parameters //
  setAsNumericalScalar( "HyperbolicAnisotropicEnumerateFunction-DefaultQ", 0.4 );

  // MarginalTransformationEvaluation parameters //
  setAsNumericalScalar( "MarginalTransformationEvaluation-DefaultTailThreshold", 0.99 );

  // DistributionImplementation parameters //
  setAsUnsignedLong( "DistributionImplementation-DefaultPointNumber", 129 );
  setAsNumericalScalar( "DistributionImplementation-DefaultQuantileEpsilon", 1.0e-12 );
  setAsNumericalScalar( "DistributionImplementation-DefaultPDFEpsilon", 1.0e-14 );
  setAsNumericalScalar( "DistributionImplementation-DefaultCDFEpsilon", 1.0e-14 );
  setAsUnsignedLong( "DistributionImplementation-DefaultQuantileIteration", 100 );
  setAsNumericalScalar( "DistributionImplementation-QMin", 0.15 );
  setAsNumericalScalar( "DistributionImplementation-QMax", 0.85 );
  setAsUnsignedLong( "DistributionImplementation-DefaultIntegrationNodesNumber", 255 );
  setAsUnsignedLong( "DistributionImplementation-DefaultLevelNumber", 10 );
  setAsUnsignedLong( "DistributionImplementation-DefaultQuantileCacheSize", 128 );

  // ContinuousDistribution parameters //
  setAsUnsignedLong( "ContinuousDistribution-DefaultIntegrationNodesNumber", 51 );

  // DiscreteDistribution parameters //
  setAsNumericalScalar( "DiscreteDistribution-SupportEpsilon", 1.0e-14 );

  // DistributionImplementationFactory parameters //
  setAsUnsignedLong( "DistributionImplementationFactory-DefaultBootstrapSize", 100 );

  // WhiteNoise parameters //
  setAsNumericalScalar( "WhiteNoise-MeanEpsilon", 1.0e-14 );

  // TemporalNormalProcess parameters //
  setAsNumericalScalar( "TemporalNormalProcess-StartingScaling", 1.0e-13 );
  setAsNumericalScalar( "TemporalNormalProcess-MaximalScaling", 1.0e5 );

  // SpectralNormalProcess parameters //
  setAsNumericalScalar( "SpectralNormalProcess-StartingScaling", 1.0e-13 );
  setAsNumericalScalar( "SpectralNormalProcess-MaximalScaling", 1.0e5 );
  setAsUnsignedLong( "SpectralNormalProcess-CholeskyCacheSize", 16384 );

  // WhittleFactory parameters //
  setAsNumericalScalar( "WhittleFactory-DefaultRhoBeg", 0.1 );
  setAsNumericalScalar( "WhittleFactory-DefaultRhoEnd", 1.0e-10 );
  setAsNumericalScalar( "WhittleFactory-DefaultMaxFun", 2000 );
  setAsNumericalScalar( "WhittleFactory-DefaultStartingPointScale", 1.0 );
  setAsNumericalScalar( "WhittleFactory-RootEpsilon", 1.0e-6);

  // BoxCoxFactory parameters //
  setAsNumericalScalar( "BoxCoxFactory-DefaultRhoBeg", 0.1 );
  setAsNumericalScalar( "BoxCoxFactory-DefaultRhoEnd", 1.0e-10 );
  setAsUnsignedLong( "BoxCoxFactory-DefaultMaxFun", 2000 );

  // VisualTest parameters //
  setAsUnsignedLong( "VisualTest-KendallPlot-MonteCarloSize", 100 );

  // CalibrationStrategyImplementation parameters //
  setAsNumericalScalar( "CalibrationStrategyImplementation-DefaultLowerBound", 0.117 ); // = 0.5 * 0.234
  setAsNumericalScalar( "CalibrationStrategyImplementation-DefaultUpperBound", 0.468 ); // = 2.0 * 0.234
  setAsNumericalScalar( "CalibrationStrategyImplementation-DefaultShrinkFactor", 0.8 );
  setAsNumericalScalar( "CalibrationStrategyImplementation-DefaultExpansionFactor", 1.2 );
  setAsUnsignedLong( "CalibrationStrategyImplementation-DefaultCalibrationStep", 500 );

  // MCMC parameters //
  setAsUnsignedLong( "MCMC-DefaultBurnIn", 2000 );
  setAsUnsignedLong( "MCMC-DefaultThinning", 100 );


  // ARMALikelihoodFactory parameters //
  setAsNumericalScalar( "ARMALikelihoodFactory-StartingScaling", 1.0e-13 );
  setAsNumericalScalar( "ARMALikelihoodFactory-MaximalScaling", 1.0e5 );
  setAsNumericalScalar( "ARMALikelihoodFactory-DefaultRhoBeg", 0.01 );
  setAsNumericalScalar( "ARMALikelihoodFactory-DefaultRhoEnd", 1.0e-10 );
  setAsUnsignedLong( "ARMALikelihoodFactory-DefaultMaxFun", 10000 );
  setAsNumericalScalar( "ARMALikelihoodFactory-DefaultStartingPointScale", 1.0 );
  setAsNumericalScalar( "ARMALikelihoodFactory-RootEpsilon", 1.0e-6);
}

/* String converter */
String ResourceMap::__repr__() const
{
  OSS oss;
  oss << "ResourceMap={";
  const char * separator = "\n  ";
  for(MapType::const_iterator it = map_.begin(); it != map_.end(); ++it, separator = ",\n  ")
    oss << separator
        << it->first << " => " << it->second;
  oss << "\n}";
  return oss;
}



/*
 * @fn std::ostream & operator <<(std::ostream & os, const ResourceMap & obj)
 * @brief Output stream converter
 * @param os A STL output stream resourceMap
 * @param obj The resourceMap read by \em os
 * @return A reference to \em os
 *
 * Operator << converts the ResourceMap object to an output stream
 * so it is easy to show the content of the resourceMap.
 */
std::ostream & operator <<(std::ostream & os, const ResourceMapInstance & obj)
{
  return os << obj.lock().__repr__();
}

OStream & operator <<(OStream & OS, const ResourceMapInstance & obj)
{
  return OS << obj.lock().__repr__();
}





END_NAMESPACE_OPENTURNS
