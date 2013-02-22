import openturns as ot

def GetAllContinuousFactories():
    """ 
    Return a dictionary with DistributionFactory objects of OT which generate
    continuous distributions
    """
    # Current list includes parametric and non parametric
    try:
        ParamNonParamContinuousFactory = ot.DistributionFactory.GetContinuousUniVariateFactories()
    except AttributeError:
        #  DistributionFactory.GetContinuousUniVariateFactories() does not exist before OT 1.2
        #  Emulate it by looping over all distributions
        ParamNonParamContinuousFactory =[]
        for elt in dir(ot.dist):
            if elt.endswith('Factory'):
                factoryName = 'ot.' + elt
                StrDist = factoryName.replace('Factory','()')
                Dist = eval(StrDist)
                if Dist.isContinuous():
                    factory = eval(factoryName+'()')
                    #  WARNING: Mimic ot.DistributionFactory.GetContinuousUniVariateFactories()
                    ParamNonParamContinuousFactory.append(ot.DistributionFactory(factory))

    #  Filter out HistogramFactory
    AllContinuousFactory = []
    for dist in ParamNonParamContinuousFactory:
        factoryName = dist.getImplementation().getClassName()
        if (factoryName != 'HistogramFactory'):
            AllContinuousFactory.append(dist)

    return AllContinuousFactory

def GetAllDiscreteFactories():
    """
    Return a dictionary with DistributionFactory objects of OT which generate
    discrete distributions
    """
    try:
        AllDiscreteFactory = ot.DistributionFactory.GetDiscreteUniVariateFactories()
    except AttributeError:
        AllDiscreteFactory =[]
        for elt in dir(ot.dist):
            if elt.endswith('Factory'):
                factoryName = 'ot.' + elt
                StrDist = factoryName.replace('Factory','()')
                Dist = eval(StrDist)
                if not Dist.isContinuous():
                    factory = eval(factoryName+'()')
                    #  WARNING: Mimic ot.DistributionFactory.GetContinuousUniVariateFactories()
                    AllDiscreteFactory.append(ot.DistributionFactory(factory))

    return AllDiscreteFactory


if __name__ == "__main__":
    continuousFactories = GetAllContinuousFactories()
    print("Factories: %s" % continuousFactories)
    for dist in continuousFactories:
        print(" factory name: %s" % dist.getImplementation().getClassName())

