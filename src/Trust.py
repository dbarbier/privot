#  This Paraview macro chains filters in order to compute trust levels.
#  Input: a table stored in an unstructured grid.
#  Filters:
#  + IsoVolume:
#      Compute an isosurface between lower and upper thresholds
#  + IntegrateVariables:
#      Compute the volume of the isosurface
#  + ProgrammableFilter:
#      Compute the bounding box of the isosurface, and its volume
#  + AppendAttributes:
#      Only needed to store the next filter
#  + Calculator:
#      Computes volume/bounding box volume
#
#  To load this macro:
#  Menu Macros/Add new macro..., then select this file.  This macro is imported,
#  and can be called from menubar or the Macros menu.
#  A table stored in an unstructured grid must be selected when this macro is called.

try: paraview.simple
except: from paraview.simple import *

AnimationScene1 = GetAnimationScene()

# Create a new Layout
new_layout = servermanager.misc.ViewLayout(registrationGroup="layouts")

# Create a 3D View
Viewer3dView1 = CreateRenderView()

# Create a SpreedSheet View
SpreadSheetView1 = paraview.simple._create_view("SpreadSheetView")

Render()

TableToStructuredGrid1 = GetActiveSource()
arrayName = TableToStructuredGrid1.PointData.GetArray(0).Name
rangeMin, rangeMax = TableToStructuredGrid1.PointData.GetArray(0).GetRange()

IsoVolume2 = IsoVolume()
IsoVolume2.ThresholdRange = [rangeMin, rangeMax]
IsoVolume2.InputScalars = ['POINTS', arrayName]

Show(IsoVolume2, Viewer3dView1)

IntegrateVariables2 = IntegrateVariables()

Show(IntegrateVariables2, SpreadSheetView1)

SetActiveSource(IsoVolume2)
ProgrammableFilter3 = ProgrammableFilter()

ProgrammableFilter3.CopyArrays = 1
ProgrammableFilter3.RequestUpdateExtentScript = ''
ProgrammableFilter3.PythonPath = ''
ProgrammableFilter3.RequestInformationScript = ''
ProgrammableFilter3.Script = """pdi = self.GetInput()
pdi.ComputeBounds()
xmin, xmax, ymin, ymax, zmin, zmax = pdi.GetBounds()

volume = (xmax - xmin) * (ymax - ymin) * (zmax - zmin)

newData = vtk.vtkDoubleArray()
newData.SetName("BBox Volume")
newData.InsertNextValue(volume)

newPts = vtk.vtkPoints()
newPts.InsertPoint(0, xmin, ymin, zmin)
newPts.InsertPoint(1, xmax, ymin, zmin)
newPts.InsertPoint(2, xmax, ymax, zmin)
newPts.InsertPoint(3, xmin, ymax, zmin)
newPts.InsertPoint(4, xmin, ymin, zmax)
newPts.InsertPoint(5, xmax, ymin, zmax)
newPts.InsertPoint(6, xmax, ymax, zmax)
newPts.InsertPoint(7, xmin, ymax, zmax)

ids = vtk.vtkIdList()
for i in xrange(8):
    ids.InsertId(i,i)
newCells = vtk.vtkHexahedron()

pdo = self.GetOutput()
pdo.SetPoints(newPts)
pdo.Allocate(1, 1) 
pdo.InsertNextCell(newCells.GetCellType(), ids)
pdo.GetCellData().AddArray(newData)
"""

Show(ProgrammableFilter3, Viewer3dView1)
Show(ProgrammableFilter3, SpreadSheetView1)

AppendAttributes2 = AppendAttributes( Input=[ IntegrateVariables2, ProgrammableFilter3 ] )

Calculator2 = Calculator()

Calculator2.Function = '-Volume/BBox Volume'
Calculator2.AttributeMode = 'Cell Data'
Calculator2.ResultArrayName = 'Trust'

Show(Calculator2, SpreadSheetView1)

Render()

DataRepresentation3 = GetDisplayProperties(ProgrammableFilter3, Viewer3dView1)
DataRepresentation3.Representation = 'Outline'

DataRepresentation4 = GetDisplayProperties(Calculator2, SpreadSheetView1)
DataRepresentation4.FieldAssociation = 'Cell Data'

Viewer3dView1.ResetCamera()
Render()

