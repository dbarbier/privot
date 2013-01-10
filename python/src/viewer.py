#                                               -*- Python -*-
#
#  @file  viewer.py
#  @brief Script to plot OpenTURNS graphs
#
#  Copyright (C) 2005-2013 EDF-EADS-Phimeca
#
#  This program is free software; you can redistribute it and/or
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  along with this library.  If not, see <http://www.gnu.org/licenses/>.
#

"""
    OpenTURNS viewer
    =============================
    graph viewer using matplotlib

    Example
    --------
    >>> import openturns as ot
    >>> from openturns.viewer import View
    >>> graph = ot.Normal().drawPDF()
    >>> view = View(graph, plot_kwargs={'color':'blue'})
    >>> view.save('curve.png', dpi=100)
    >>> view.show(block=False)

"""

import openturns as ot
import numpy as np
import matplotlib.pyplot as pyplot
import re
import warnings

class View:
    """
    View(graph, **kwargs)

    Create the matplotlib figure.

    Parameters
    ----------
    graph:
        An OpenTURNS Graph object, or else a Drawable object.

    plot_kwargs:
        Used when drawing Cloud, Curve drawables
        Passed on as matplotlib.axes.Axes.plot kwargs

    axes_kwargs:
        Passed on as matplotlib.figure.Figure.add_subplot kwargs

    bar_kwargs:
        Used when drawing BarPlot drawables
        Passed on as matplotlib.pyplot.bar kwargs

    pie_kwargs:
        Used when drawing Pie drawables
        Passed on as matplotlib.pyplot.pie kwargs

    contour_kwargs:
        Used when drawing Contour drawables
        Passed on as matplotlib.pyplot.contour kwargs

    clabel_kwargs:
        Used when drawing Contour drawables
        Passed on as matplotlib.pyplot.clabel kwargs

    step_kwargs:
        Used when drawing Staircase drawables
        Passed on as matplotlib.pyplot.step kwargs

    text_kwargs:
        Used when drawing Pairs drawables
        Passed on as matplotlib.axes.Axes.text kwargs

    legend_kwargs:
        Passed on as matplotlib.axes.Axes.legend kwargs
    """

    # check that the argument is a dictionnary
    @staticmethod
    def CheckDict(arg):
        result = arg
        if arg is None:
            result = dict()
        elif not isinstance(arg, dict):
            raise TypeError( 'Argument is not a dict' )
        return result

    # constructor
    def __init__( self,
                  graph,
                  plot_kwargs=None,
                  axes_kwargs=None,
                  bar_kwargs=None,
                  pie_kwargs=None,
                  contour_kwargs=None,
                  step_kwargs=None,
                  clabel_kwargs=None,
                  text_kwargs=None,
                  legend_kwargs=None,
                  **kwargs):
        try:
            try:
                graph = ot.Graph(graph)
            except:
                drawable = ot.Drawable(graph)
                graph = ot.Graph()
                graph.add(drawable)
        except:
            raise RuntimeError( '-- The given object cannot be converted into an OpenTURNS Graph nor Drawable.' )
            return

        drawables = graph.getDrawables()
        size = len(drawables)
        if size == 0:
            warnings.warn( '-- Nothing to draw.' )
            return

        # check that arguments are dictionnaries
        axes_kwargs = View.CheckDict(axes_kwargs)
        plot_kwargs_default = View.CheckDict(plot_kwargs)
        bar_kwargs_default = View.CheckDict(bar_kwargs)
        pie_kwargs_default = View.CheckDict(pie_kwargs)
        contour_kwargs_default = View.CheckDict(contour_kwargs)
        step_kwargs_default = View.CheckDict(step_kwargs)
        clabel_kwargs_default = View.CheckDict(clabel_kwargs)
        text_kwargs_default = View.CheckDict(text_kwargs)
        legend_kwargs = View.CheckDict(legend_kwargs)

        # set title
        axes_kwargs.setdefault('title', graph.getTitle())

        # set scale
        if (graph.getLogScale() == ot.GraphImplementation.LOGX) or (graph.getLogScale() == ot.GraphImplementation.LOGXY):
            axes_kwargs.setdefault('xscale','log')
        if (graph.getLogScale() == ot.GraphImplementation.LOGY) or (graph.getLogScale() == ot.GraphImplementation.LOGXY):
            axes_kwargs.setdefault('yscale','log')

        # set bounding box
        axes_kwargs.setdefault('xlim', [ graph.getBoundingBox()[0], graph.getBoundingBox()[1] ] )
        axes_kwargs.setdefault('ylim', [ graph.getBoundingBox()[2], graph.getBoundingBox()[3] ] )

        self._fig = pyplot.figure()
        self._ax = [ self._fig.add_subplot(111, **axes_kwargs) ]
        self._ax[0].grid()

        for drawable in drawables:
            # reset working dictionaries by excplicitely creating copies
            plot_kwargs = dict(plot_kwargs_default)
            bar_kwargs = dict(bar_kwargs_default)
            pie_kwargs = dict(pie_kwargs_default)
            contour_kwargs = dict(contour_kwargs_default)
            step_kwargs = dict(step_kwargs_default)
            clabel_kwargs = dict(clabel_kwargs_default)
            text_kwargs = dict(text_kwargs_default)

            drawableKind = re.search('(?<=implementation\=class\=)\w+', re.sub('implementation=\s?class', 'implementation=class', repr(drawable)[:70])).group(0)

            # set color
            if (not 'color' in plot_kwargs_default) and (not 'c' in plot_kwargs_default):
                plot_kwargs['color'] = drawable.getColorCode()
            if (not 'color' in bar_kwargs_default) and (not 'c' in bar_kwargs_default):
                bar_kwargs['color'] = drawable.getColorCode()
            if (not 'color' in step_kwargs_default) and (not 'c' in step_kwargs_default):
                step_kwargs['color'] = drawable.getColorCode()

            # set marker
            pointStyleDict = {'square':'s', 'circle':'o', 'triangleup':'2', 'plus':'+', 'times':'+', 'diamond':'+', 'triangledown':'v', 'star':'*', 'fsquare':'s', 'fcircle':'o', 'ftriangleup':'2', 'fdiamond':'D', 'bullet':'+', 'dot':','}
            if not 'marker' in plot_kwargs_default:
                try:
                    plot_kwargs['marker'] = pointStyleDict[ drawable.getPointStyle() ]
                except:
                    warnings.warn( '-- Unknown marker: ' + drawable.getPointStyle() )

            # set line style
            lineStyleDict = {'solid':'-', 'dashed':'--', 'dotted':':', 'dotdash':'-.', 'longdash':'--', 'twodash':'--'}
            if (not 'linestyle' in plot_kwargs_default) and (not 'ls' in plot_kwargs_default):
                try:
                    plot_kwargs['linestyle'] = lineStyleDict[ drawable.getLineStyle() ]
                except:
                    warnings.warn( '-- Unknown line style' )
            if (not 'linestyle' in step_kwargs_default) and (not 'ls' in step_kwargs_default):
                try:
                    step_kwargs['linestyle'] = lineStyleDict[ drawable.getLineStyle() ]
                except:
                    warnings.warn( '-- Unknown line style' )

            # set line width
            if (not 'linewidth' in plot_kwargs_default) and (not 'lw' in plot_kwargs_default):
                plot_kwargs['linewidth'] = drawable.getLineWidth()
            if (not 'linewidth' in step_kwargs_default) and (not 'lw' in step_kwargs_default):
                step_kwargs['linewidth'] = drawable.getLineWidth()

            # retrieve data
            data = drawable.getData()
            x = data.getMarginal(0)
            if data.getDimension()>1:
                y = data.getMarginal(1)

            # add legend, title
            if drawableKind != 'Pie':
                self._ax[0].set_xlabel( graph.getXTitle() )
                self._ax[0].set_ylabel( graph.getYTitle() )

                legend_name = '_nolegend_'
                if len(drawable.getLegendName()) > 0:
                    legend_name = drawable.getLegendName()

                plot_kwargs.setdefault('label', legend_name)
                bar_kwargs.setdefault('label', legend_name)
                step_kwargs.setdefault('label', legend_name)

            if drawableKind == 'BarPlot':
                # linestyle for bar() is different than the one for plot()
                if 'linestyle' in bar_kwargs_default:
                    bar_kwargs.pop('linestyle')
                if (not 'linestyle' in plot_kwargs_default) and (not 'ls' in plot_kwargs_default):
                    lineStyleDict = {'solid':'solid', 'dashed':'dashed', 'dotted':'dotted', 'dotdash':'dashdot', 'longdash':'dashed', 'twodash':'dashed'}
                    if drawable.getLineStyle() in lineStyleDict:
                        bar_kwargs['linestyle'] = lineStyleDict[ drawable.getLineStyle() ]
                    else:
                        warnings.warn( '-- Unknown line style: '+ drawable.getLineStyle() )

                xi = drawable.getOrigin()
                for i in range(x.getSize()):
                    # label only the first bar to avoid getting several legend items
                    if (i == 1) and ('label' in bar_kwargs):
                        bar_kwargs.pop('label')
                    pyplot.bar( xi, height=y[i][0], width=x[i][0], **bar_kwargs )
                    xi += x[i][0]

            elif drawableKind == 'Cloud':
                plot_kwargs['linestyle'] = 'None'
                self._ax[0].plot(x, y, **plot_kwargs)

            elif drawableKind == 'Curve':
                if plot_kwargs['linestyle'] == '-':
                    plot_kwargs['marker'] = ''
                self._ax[0].plot( x, y, **plot_kwargs )

            elif drawableKind == 'Pie':
                pie_kwargs.setdefault('labels', drawable.getLabels())
                pie_kwargs.setdefault('colors', drawable.getPalette())
                self._ax[0].set_aspect('equal')
                pyplot.pie( x, **pie_kwargs )

            elif drawableKind == 'Contour':
                X, Y = np.meshgrid( drawable.getX(), drawable.getY() )
                Z = np.reshape(drawable.getData(), (drawable.getX().getSize(), drawable.getY().getSize()) )

                contour_kwargs.setdefault('levels', drawable.getLevels())
                contourset = pyplot.contour(X, Y, Z, **contour_kwargs)

                clabel_kwargs.setdefault('fontsize', 8)
                clabel_kwargs.setdefault('fmt','%g')
                pyplot.clabel( contourset, **clabel_kwargs )

            elif drawableKind == 'Staircase':
                pyplot.step( x, y, **step_kwargs )

            elif drawableKind == 'Pairs':
                # disable axis : grid, ticks, axis
                self._ax[0].axison = False

                if 'title' in axes_kwargs:
                    axes_kwargs.pop('title')
                axes_kwargs['xticks'] = []
                axes_kwargs['yticks'] = []

                dim = drawable.getData().getDimension()

                # adjust font
                if (not 'fontsize' in text_kwargs_default) and (not 'size' in text_kwargs_default):
                    text_kwargs['fontsize'] = max( 16 - dim, 4 )
                text_kwargs.setdefault('horizontalalignment', 'center')
                text_kwargs.setdefault('verticalalignment', 'center')
                for i in range(dim):
                    for j in range(dim):
                        self._ax.append( self._fig.add_subplot( dim, dim, 1+i*dim+j, **axes_kwargs ) )
                        if i != j:
                            x = drawable.getData().getMarginal(i)
                            y = drawable.getData().getMarginal(j)
                            plot_kwargs['linestyle'] = 'None'
                            self._ax[1+i*dim+j].plot( x, y, **plot_kwargs )
                            self._ax[1+i*dim+j].set_xlim( x.getMin()[0], x.getMax()[0] )
                            self._ax[1+i*dim+j].set_ylim( y.getMin()[0], y.getMax()[0] )
                        else:
                            text_kwargs['transform'] = self._ax[1+i*dim+j].transAxes
                            self._ax[1+i*dim+j].text( 0.5,0.5,'marginal '+str(i+1), **text_kwargs )

        # Add legend
        if (drawableKind != 'Pie') and (graph.getLegendPosition() != ''):
            # set legend position
            if not 'loc' in legend_kwargs:
                try:
                    legendPositionDict = {'bottomright':'lower right', 'bottom':'lower center', 'bottomleft':'lower left', 'left':'center left', 'topleft':'upper left', 'topright':'upper right', 'right':'center right', 'center':'center'}
                    legend_kwargs['loc'] = legendPositionDict[ graph.getLegendPosition() ]
                except:
                    warnings.warn( '-- Unknown legend position: ' + graph.getLegendPosition() )

            # set a single legend point
            legend_kwargs.setdefault( 'numpoints', 1 )

            # enable round box by default
            legend_kwargs.setdefault('fancybox', True)

            # enable shadow by default
            legend_kwargs.setdefault('shadow', True)

            self._ax[0].legend( **legend_kwargs )

    def show(self, **kwargs):
        """
        show(**kwargs)

        Display the graph on screen.

        Parameters
        ----------
        kwargs:
            block: bool, optional
                If true (default), block until the graph is closed.

            These parameters are passed to matplotlib.pyplot.show()
        """

        pyplot.show(**kwargs)

    def save(self, fname, **kwargs):
        """
        save(fname, **kwargs)

        Display the graph on screen.

        Parameters
        ----------
        fname: bool, optional
            A string containing a path to a filename from which file format is deduced.

        kwargs:
            Refer to matplotlib.figure.Figure.savefig documentation for valid keyword arguments.
        """

        self._fig.savefig(fname, **kwargs)

    def getFigure(self):
        """
        getFigure()

        Accessor to the underlying figure object.

        Refer to matplotlib.Figure for further information.
        """
        return self._fig

    def getAxes(self):
        """
        getAxes()

        Accessor to the list of Axes objects.

        Refer to matplotlib.axes.Axes for further information.
        """
        return self._ax
