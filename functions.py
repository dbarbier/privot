import openturns as ot


def GetAllContinuousFactories():
    """
    Return a dictionary with DistributionFactory objects of OT which generate
    continuous distributions
    """
    # Current list includes parametric and non parametric
    try:
        factories = ot.DistributionFactory.GetContinuousUniVariateFactories()
    except AttributeError:
        #  DistributionFactory.GetContinuousUniVariateFactories() does not
        #  exist before OT 1.2.
        #  Emulate it by looping over all distributions
        factories = []
        for elt in dir(ot.dist):
            if elt.endswith('Factory'):
                factory_name = 'ot.' + elt
                str_dist = factory_name.replace('Factory', '()')
                dist = eval(str_dist)
                if dist.isContinuous():
                    factory = eval(factory_name + '()')
                    #  WARNING: Mimic GetContinuousUniVariateFactories()
                    factories.append(ot.DistributionFactory(factory))

    #  Filter out HistogramFactory
    continuous_factories = []
    for factory in factories:
        factory_name = factory.getImplementation().getClassName()
        if (factory_name != 'HistogramFactory'):
            continuous_factories.append(factory)

    return continuous_factories


def GetAllDiscreteFactories():
    """
    Return a dictionary with DistributionFactory objects of OT which generate
    discrete distributions
    """
    try:
        factories = ot.DistributionFactory.GetDiscreteUniVariateFactories()
    except AttributeError:
        factories = []
        for elt in dir(ot.dist):
            if elt.endswith('Factory'):
                factory_name = 'ot.' + elt
                str_dist = factory_name.replace('Factory', '()')
                dist = eval(str_dist)
                if not dist.isContinuous():
                    factory = eval(factory_name + '()')
                    #  WARNING: Mimic GetContinuousUniVariateFactories()
                    factories.append(ot.DistributionFactory(factory))

    return factories


if __name__ == "__main__":
    CONTINUOUS_FACTORIES = GetAllContinuousFactories()
    print("Factories: %s" % CONTINUOUS_FACTORIES)
    for DIST in CONTINUOUS_FACTORIES:
        print(" factory name: %s" % DIST.getImplementation().getClassName())
