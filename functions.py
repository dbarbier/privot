import openturns as ot

def getOTVersion():
    ''' 
    Get the openturns version for PlatformInfo as a list of three integer : (major, minor, revision)
    '''
    openturnsVersion = ot.PlatformInfo.GetVersion()
    # check if current software version is a devel one!
    if (openturnsVersion.count('based-devel') > 0):
        index = openturnsVersion.index('based-devel') - 1 # (-1) because we remove -
        ot.Log.Warn('You are using a based-devel vesion')
        openturnsVersion = openturnsVersion[0:index]
    # The ot version should be numeric here, separated with '.'
    openturnsVersion = openturnsVersion.split('.')
    # Parsing and getting all elements of version
    try:
        major = openturnsVersion[0]
        majorInt = int(major)
    except:
        raise ValueError('Incorrect major number')
    try:
        minor = openturnsVersion[1]
        minorInt = int(minor)
    except:
        raise ValueError('Incorrect minor number')

    try:
        revision = openturnsVersion[2]
    except:
        UserWarning('No revision version found')
        revisionInt = 0
    return majorInt * 100 +  minorInt * 10 + revisionInt

#--------------------------------------------------#
#-- Fixing the software version that has catalog --#
#--------------------------------------------------#
major = 1
minor = 2
revision = 0

# Check if function contains catalog or if it should make it here self!
def hasCatalog(major, minor, revision):
    '''
    The function checks if the used library version has catalog or if we need to check
    swig module.
    '''
    # OpenTURNS version
    openturnsVersion = getOTVersion()
    versionOTWithCatalog  = major * 100 + minor * 10 + revision
    return openturnsVersion >= versionOTWithCatalog

if hasCatalog(major, minor, revision) is False:
    # Function to be used with version <= 1.1
    def GetAllFactories():
        """ 
        Return a dictionnary with DistributionFactory objects of OT with the distinction
        'AllContinuousFactory' and 'AllDiscreteFactory'
        Care! May be incorrect with too older version because of SWIG modules were
        differents (no dist maybe?)
        """
        allDist = dir(ot.dist)
        result = {}
        AllContinuousFactory =[]
        AllDiscreteFactory =[]
        excludedFactories = ['HistogramFactory']
        for elt in allDist:
            if elt.endswith('Factory') and elt not in excludedFactories:
                factoryName = 'ot.' + elt
                StrDist = factoryName.replace('Factory','()')
                Dist = eval(StrDist)
                if Dist.isContinuous():
                    factory = eval(factoryName+'()')
                    AllContinuousFactory.append(factory)
                else :
                    factory = eval(factoryName+'()')
                    AllDiscreteFactory.append(factory)
        result['AllContinuousFactory'] = AllContinuousFactory
        result['AllDiscreteFactory'] = AllDiscreteFactory
        return(result)

    resultAllFactory = GetAllFactories()

    def GetAllContinuousFactories():
        return resultAllFactory['AllContinuousFactory']

    def GetAllDiscreteFactories():
        return resultAllFactory['AllDiscreteFactory']

else :
    # Function to be used with version >= 1.2
    def GetAllContinuousFactories():
        """ 
        Return a dictionnary with DistributionFactory objects of OT with the distinction
        'AllContinuousFactory' and 'AllDiscreteFactory'
        """
        # Current list includes parametric and non parametric
        ParamNonParamContinuousFactory = ot.DistributionFactory.GetContinuousUniVariateFactories()
        AllContinuousFactory = []
        for dist in ParamNonParamContinuousFactory:
            factoryName = dist.getImplementation().getClassName()
            if (factoryName != 'HistogramFactory'):
                AllContinuousFactory.append(dist)

        return AllContinuousFactory

    def GetAllDiscreteFactories():
        """ Return a dictionnary with DistributionFactory objects of OT with the distinction
        'AllDiscreteFactory' and 'AllDiscreteFactory'
        """
        return ot.DistributionFactory.GetDiscreteUniVariateFactories()


if __name__ == "__main__":
    print("DistributionFactory has method GetDiscreteUniVariateFactories()? "+str(hasCatalog(major, minor, revision)))
    continuousFactories = GetAllContinuousFactories()
    print("Factories: %s" % continuousFactories)
    for dist in continuousFactories:
        print(" factory name: %s" % dist.getImplementation().getClassName())

