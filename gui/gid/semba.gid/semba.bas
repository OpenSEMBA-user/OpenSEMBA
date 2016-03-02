=======================================================================
Format: Semba Data File
Version: *tcl(set version $semba::VersionNumber)
=======================================================================

Solver options: *GenData(Solver)
Run simulation: *GenData(Run_simulation)
*if(strcasecmp(GenData(Ending),"Final_time")==0)
Final time: *GenData(Final_time)
*else
Number of time steps: *GenData(Number_of_time_steps)
*endif
Time step: *GenData(Time_step)
CFL: *GenData(CFL)
Default sampling period: *GenData(Default_sampling_period)
Force restarting: *GenData(Force_restarting)
Resume simulation: *GenData(Resume_simulation)
Flush: *GenData(Flush)
*if(strcmp(GenData(Additional_arguments),"")!=0)
Additional arguments: *GenData(Additional_arguments)
*endif
*if(strcasecmp(GenData(Solver),"ugrfdtd")==0)
Composites model: *GenData(Composites_model)
Conformal skin: *GenData(Conformal_skin)
No compo Mur: *GenData(No_compo_mur)
Skin depth: *GenData(Skin_depth)
Composites attenuation factor: *GenData(Composites_attenuation_factor)

Wires flavor: *GenData(Wires_flavor)
MTLN: *GenData(MTLN)
Min distance wires: *GenData(Min_distance_wires)
New dispersive formulation: *GenData(New_dispersive_formulation)
Taparrabos: *GenData(Taparrabos)
Make holes: *GenData(Make_holes)
Ground wires: *GenData(Ground_wires)
Inductance model: *GenData(Inductance_model)
Wires attenuation factor: *GenData(Wires_attenuation_factor)

Use default PML: *GenData(Use_default_PML)
PML alpha: *GenData(PML_alpha_factor) *GenData(PML_alpha_order)
PML kappa: *GenData(PML_kappa)
PML correction: *GenData(PML_correction_factor) *GenData(PML_correction_depth)
PML backing: *GenData(PML_backing)

Map: *GenData(Map)
Map VTK: *GenData(Map_VTK)
NF2FF decimation: *GenData(NF2FF_decimation)
Geometry scaling factor: *GenData(scaling_factor)

Mesher options:
Upper x bound: *GenData(Upper_x_bound)
Lower x bound: *GenData(Lower_x_bound)
Upper y bound: *GenData(Upper_y_bound)
Lower y bound: *GenData(Lower_y_bound)
Upper z bound: *GenData(Upper_z_bound)
Lower z bound: *GenData(Lower_z_bound)
Mesher: *GenData(Mesher)
Brute force volumes: *GenData(Brute_force_volumes)
VTK Export: *GenData(VTK_Export)
postmsh Export: *GenData(postmsh_Export)
Mode: *GenData(Mode)
Forbidden length: *GenData(Forbidden_length)
Scale factor value: *GenData(Scale_factor_value)
End of Mesher options
*elseif(strcasecmp(GenData(Solver),"cudg3d")==0)
cudg3d options:
Time integrator: *GenData(Time_integrator)
Use LTS: *GenData(Use_LTS)

Upwinding: *GenData(Upwinding)
*else
*WarningBox Unrecognized solver
*endif
End of Solver options

Problem size:
*set elems(Hexahedra)
Hexahedral elements: *nelem
*set elems(Tetrahedra)
Tetrahedral elements: *nelem
*set elems(Triangle)
Triangular elements: *nelem
*set elems(Linear)
Line elements: *nelem
*set elems(all)
Number of points: *npoin
Number of materials: *nmats
End of problem size:

Materials:
*loop materials
Material: *matnum()
Name: *MatProp(0)
TypeId: *MatProp(TypeId)
*if(strcmp(MatProp(TypeId),"PEC")==0)
*endif
*if(strcmp(MatProp(TypeId),"PMC")==0)
*endif
*if(strcmp(MatProp(TypeId),"PML")==0)
Autmatic Orientation: *matprop(Automatic_orientation)
*if(strcmp(MatProp(Local_Axes),"-GLOBAL-")==0)
Local Axes: {0.0 0.0 0.0} {0.0 0.0 0.0}
*else
Local Axes: *tcl(GiD_Info localaxes *matprop(Local_Axes))
*endif
*endif
*if(strcmp(Matprop(TypeId),"Classic")==0)
Permittivity: *matprop(Permittivity)
Permeability: *matprop(Permeability)
Electric Conductivity: *matprop(ElecCond)
Magnetic Conductivity: *matprop(MagnCond)
*endif
*if(strcmp(Matprop(TypeId),"Dispersive")==0)
Filename: *matprop(File)
*endif
*if(strcmp(Matprop(TypeId),"Wire")==0)
Radius: *matprop(Radius)
Resistance: *matprop(Resistance)
Inductance: *matprop(Inductance)
*endif
*if(strcmp(Matprop(TypeId),"Conn_sRLC")==0)
Resistance: *matprop(Resistance)
Inductance: *matprop(Inductance)
Capacitance: *matprop(Capacitance)
*endif
*if(strcmp(Matprop(TypeId),"Conn_pRLC")==0)
Resistance: *matprop(Resistance)
Inductance: *matprop(Inductance)
Capacitance: *matprop(Capacitance)
*endif
*if(strcmp(Matprop(TypeId),"Conn_sLpRC")==0)
Resistance: *matprop(Resistance)
Inductance: *matprop(Inductance)
Capacitance: *matprop(Capacitance)
*endif
*if(strcmp(Matprop(TypeId),"SIBC")==0)
SurfaceType: *matprop(SurfaceType)
Filename: *matprop(File)
Layers: *matprop
*endif
*if(strcmp(Matprop(TypeId),"Anisotropic")==0)
*if(strcmp(MatProp(Local_Axes),"-GLOBAL-")==0)
Local Axes: {0.0 0.0 0.0} {0.0 0.0 0.0}
*else
Local Axes: *tcl(GiD_Info localaxes *matprop(Local_Axes))
*endif
Anisotropic model: *matprop(Anisotropic_model)
Relative permittivity principal axes: *matprop(Relative_permittivity_principal_axes)
Crystal relative permeability: *matprop(Crystal_relative_permeability)
Kappa: *matprop(Kappa)
Ferrite relative permeability: *matprop(Ferrite_relative_permeability)
Ferrite relative permittivity: *matprop(Ferrite_relative_permittivity)
*endif
End of Material:
*end materials
End of materials:

Grid:
*set elems(all)
*set Cond Grid
*if(CondNumEntities(int)>0)
*loop layers *OnlyInCond
 Layer Box: *tcl(GiD_Info layer -bbox -use geometry *layerName)
 Type: *cond(Type)
 Directions: *cond(x_direction) *cond(y_direction) *cond(z_direction)
*end layers
*endif
Boundary padding type: *GenData(boundary_padding_type)
Boundary padding: {*GenData(Upper_x_boundary_padding) *GenData(Upper_y_boundary_padding) *GenData(Upper_z_boundary_padding) *GenData(Lower_x_boundary_padding) *GenData(Lower_y_boundary_padding) *GenData(Lower_z_boundary_padding)}
Boundary mesh size: {*GenData(Upper_x_boundary_mesh_size) *GenData(Upper_y_boundary_mesh_size) *GenData(Upper_z_boundary_mesh_size) *GenData(Lower_x_boundary_mesh_size) *GenData(Lower_y_boundary_mesh_size) *GenData(Lower_z_boundary_mesh_size)}
End of Grid

Layers:
*set elems(all)
*loop layers
*LayerNum *LayerName
*end layers
End of layers

Coordinates:
*set elems(all)
*loop nodes
*format "%7i %+14.8e %+14.8e %+14.8e"
*NodesNum *NodesCoord(1,real) *NodesCoord(2,real) *NodesCoord(3,real)
*end nodes
End of coordinates

Elements:
*set elems(Hexahedra)
Hexahedral Elements:
*loop elems
*format "%8i %7i %7i %7i %7i %7i %7i %7i %7i %3i %3i"
*ElemsNum *ElemsConec *ElemsMat *ElemsLayerNum 
*end elems
End of Hexahedral Elements
*set elems(Tetrahedra)
Tetrahedral Elements:
*loop elems
*format "%8i %7i %7i %7i %7i %3i %3i"
*ElemsNum *ElemsConec(1) *elemsconec(3) *elemsconec(2) *elemsconec(4) *ElemsMat *ElemsLayerNum
*end elems
End of Tetrahedral Elements
*set elems(Triangle)
Triangle Elements:
*loop elems
*format "%8i %7i %7i %7i %3i %3i"
*ElemsNum *ElemsConec(1) *elemsconec(2) *elemsconec(3) *ElemsMat *ElemsLayerNum 
*end elems
End of Triangle Elements
*set elems(Linear)
Line Elements:
*loop elems
*format "%8i %7i %7i %3i %3i"
*ElemsNum *ElemsConec(1) *ElemsConec(2) *ElemsMat *ElemsLayerNum
*End elems
End of Line Elements
End of Elements

Excitations:
*set elems(all)
*Set Cond Planewave
*if(CondNumEntities(int)>0)
Planewave: 
*loop layers *OnlyInCond
Direction: *cond(Direction_x) *cond(Direction_y) *cond(Direction_z) 
Polarization: *cond(Polarization_x) *cond(Polarization_y) *cond(Polarization_z) 
Excitation: *cond(Excitation)
Gaussian spread: *cond(Gaussian_spread)
Gaussian delay: *cond(Gaussian_delay)
Filename: *cond(File)
Defined: OnLayers
Layer Name: *layerName
Layer Box: *tcl(GiD_Info layer -bbox -use geometry *layerName)
*end layers
End of Planewave:
*endif

*loop conditions *bodyElements
*if((strcasecmp(condName,"planewave_on_volume")==0)&&CondNumEntities(int)>0)
 Planewave: 
 Direction: *cond(Direction_x) *cond(Direction_y) *cond(Direction_z) 
 Polarization: *cond(Polarization_x) *cond(Polarization_y) *cond(Polarization_z) 
 Excitation: *cond(Excitation)
 Gaussian spread: *cond(Gaussian_spread)
 Gaussian delay: *cond(Gaussian_delay)
 Filename: *cond(File)
 Defined: OnElements
 Number of elements: *CondNumEntities(int)
*loop elems *OnlyInCond
  *elemsNum
*end elems
End of Planewave:
*endif
*end conditions

*Set Cond Source_on_line *bodyElements
*set var HEADER=0
*loop elems *OnlyInCond
*if(HEADER==0) 
*set var HEADER=1
 Source_on_line:
 Type: *cond(Type) 
 Hardness: *cond(Hardness) 
 Excitation: *cond(Excitation)
 Gaussian spread: *cond(Gaussian_spread)
 Gaussian delay: *cond(Gaussian_delay)
 Filename: *cond(File)
 Defined: OnElements
 Number of elements: *CondNumEntities(int)
*endif
  *elemsNum
*end elems
*if(HEADER==1)
End of Source_on_line:
*endif

*loop conditions *nodes
*if(strcasecmp(condName,"Generator_on_line")==0&&CondNumEntities(int)>0)
*loop nodes *OnlyInCond
 Generator:
 Type: *cond(Type)
 Excitation: *cond(Excitation)
 Gaussian spread: *cond(Gaussian_spread)
 Gaussian delay: *cond(Gaussian_delay)
 Filename: *cond(File)
 Defined: OnNodes
 Number of elements: 1
 *NodesNum
 End of Generator:
*end nodes

*endif
*end conditions
*loop conditions *faceElements
*if(strcasecmp(condName,"Waveguide_port")==0&&condNumEntities>0)
*set var HEADER = 0
*loop elems *onlyInCond
*if(HEADER == 0)
 Waveguide_port: 
 Excitation: *cond(Excitation)
 Gaussian spread: *cond(Gaussian_spread)
 Gaussian delay: *cond(Gaussian_delay)
 Filename: *cond(File)
 ExcitationMode: *cond(Mode)
 FirstMode: *cond(FirstMode)
 SecondMode: *cond(SecondMode)
 Number of elements: *CondNumEntities(int)
 Elements:
*set var HEADER = 1
*endif
*if(CondElemFace==1)
  *ElemsNum 1
*elseif(CondElemFace==2)
  *ElemsNum 4
*elseif(CondElemFace==3)
  *ElemsNum 2
*else
  *ElemsNum 3
*endif
*end elems
 End of Waveguide_port:
*endif 

*end conditions
*loop conditions *bodyElements
*if(strcasecmp(condName,"Waveguide_port")==0&&condNumEntities>0)
*set var HEADER = 0
*loop elems *onlyInCond
*if(HEADER == 0)
 Waveguide_port: 
 Excitation: *cond(Excitation)
 Gaussian spread: *cond(Gaussian_spread)
 Gaussian delay: *cond(Gaussian_delay)
 Filename: *cond(File)
 ExcitationMode: *cond(Mode)
 FirstMode: *cond(FirstMode)
 SecondMode: *cond(SecondMode)
 Number of elements: *CondNumEntities(int)
 Elements:
*set var HEADER = 1
*endif
*if(CondElemFace==1)
  *ElemsNum 1
*elseif(CondElemFace==2)
  *ElemsNum 4
*elseif(CondElemFace==3)
  *ElemsNum 2
*else
  *ElemsNum 3
*endif
*end elems
 End of Waveguide port:
*endif 
*end conditions

*loop conditions *faceElements
*if(strcasecmp(condName,"TEM_port")==0&&condNumEntities>0)
*set var HEADER = 0
*loop elems *onlyInCond
*if(HEADER == 0)
 TEM_port: 
 Excitation: *cond(Excitation)
 Gaussian spread: *cond(Gaussian_spread)
 Gaussian delay: *cond(Gaussian_delay)
 Filename: *cond(File)
 Origin: *cond(Origin)
 Inner radius: *cond(Inner_radius)
 Outer radius: *cond(Outer_radius)
 ExcitationMode: *cond(Mode)
 Number of elements: *CondNumEntities(int)
 Elements:
*set var HEADER = 1
*endif
*if(CondElemFace==1)
  *ElemsNum 1
*elseif(CondElemFace==2)
  *ElemsNum 4
*elseif(CondElemFace==3)
  *ElemsNum 2
*else
  *ElemsNum 3
*endif
*end elems
 End of Waveguide_port:
*endif 

*end conditions
*loop conditions *bodyElements
*if(strcasecmp(condName,"TEM_port")==0&&condNumEntities>0)
*set var HEADER = 0
*loop elems *onlyInCond
*if(HEADER == 0)
 TEM_port: 
 Excitation: *cond(Excitation)
 Gaussian spread: *cond(Gaussian_spread)
 Gaussian delay: *cond(Gaussian_delay)
 Filename: *cond(File)
 Origin: *cond(Origin)
 Inner radius: *cond(Inner_radius)
 Outer radius: *cond(Outer_radius)
 ExcitationMode: *cond(Mode)
 Number of elements: *CondNumEntities(int)
 Elements:
*set var HEADER = 1
*endif
*if(CondElemFace==1)
  *ElemsNum 1
*elseif(CondElemFace==2)
  *ElemsNum 4
*elseif(CondElemFace==3)
  *ElemsNum 2
*else
  *ElemsNum 3
*endif
*end elems
 End of TEM port:
*endif 

*end conditions

End of Excitations:

Output Requests:
*Set Cond OutRq_on_point
*if(CondNumEntities(int)>0)
Output request instance:
GiDOutputType: OutRq_on_point
Number of elements: *CondNumEntities(int)
*loop nodes *OnlyInCond
Name: *cond(Name)
Type: *cond(Type) 
Domain: *cond(Time) *cond(Initial_time) *cond(Final_time) *cond(Sampling_period) *cond(Frequency) *cond(Initial_Frequency) *cond(Final_Frequency) *cond(Frequency_step) *cond(Log_frequency_sweep) *cond(Use_transfer_function) "*cond(transfer_function_file)" 
*NodesNum
*end nodes
End of Output request instance: 
*endif
*Set cond OutRq_on_surface
*if(CondNumEntities(int)>0)
Output request instance: 
GiDOutputType: OutRq_on_surface
Number of elements: *CondNumEntities(int)
*loop elems OnlyInCond
Name: *cond(Name) 
Type: *cond(Type) 
Domain: *cond(Time) *cond(Initial_time) *cond(Final_time) *cond(Sampling_period) *cond(Frequency) *cond(Initial_Frequency) *cond(Final_Frequency) *cond(Frequency_step) *cond(Log_frequency_sweep) *cond(Use_transfer_function) "*cond(transfer_function_file)" 
*elemsNum
*end elems
End of Output request instance:
*end if
*Set cond OutRq_on_volume
*if(CondNumEntities(int)>0)
Output request instance: 
GiDOutputType: OutRq_on_volume
Number of elements: *CondNumEntities(int)
*loop layers *OnlyInCond
Name: *cond(Name) 
Type: *cond(Type) 
Domain: *cond(Time) *cond(Initial_time) *cond(Final_time) *cond(Sampling_period) *cond(Frequency) *cond(Initial_Frequency) *cond(Final_Frequency) *cond(Frequency_step) *cond(Log_frequency_sweep) *cond(Use_transfer_function) "*cond(transfer_function_file)"
*tcl(GiD_Info layer -bbox -use geometry *layerName)
*end layers
End of Output request instance: 
*end if
*Set cond Bulk_current_on_surface
*if(CondNumEntities(int)>0)
Output request instance: 
GiDOutputType: Bulk_current_on_surface
Number of elements: *CondNumEntities(int)
*loop elems *OnlyInCond
Name: *cond(Name) 
Type: *cond(Type) 
Domain: *cond(Time) *cond(Initial_time) *cond(Final_time) *cond(Sampling_period) *cond(Frequency) *cond(Initial_Frequency) *cond(Final_Frequency) *cond(Frequency_step) *cond(Log_frequency_sweep) *cond(Use_transfer_function) "*cond(transfer_function_file)"
Direction: *cond(Direction)
Skip: *cond(Skip)
*elemsNum
*end elems
End of Output request instance:
*end if
*Set cond Bulk_current_on_volume
*if(CondNumEntities(int)>0)
Output request instance: 
GiDOutputType: Bulk_current_on_volume
Number of elements: *CondNumEntities(int)
*loop layers *OnlyInCond
Name: *cond(Name) 
Type: *cond(Type) 
Domain: *cond(Time) *cond(Initial_time) *cond(Final_time) *cond(Sampling_period) *cond(Frequency) *cond(Initial_Frequency) *cond(Final_Frequency) *cond(Frequency_step) *cond(Log_frequency_sweep) *cond(Use_transfer_function) "*cond(transfer_function_file)"
Direction: *cond(Direction)
Skip: *cond(Skip)
*tcl(GiD_Info layer -bbox -use geometry *layerName)
*end layers
End of Output request instance: 
*end if
*Set cond Far_field
*if(CondNumEntities(int)>0)
Output request instance: 
GiDOutputType: Far_field
Number of elements: *CondNumEntities(int)
*loop layers *OnlyInCond
Name: *cond(Name) 
Type: *cond(Type) 
Domain: *cond(Time) *cond(Initial_time) *cond(Final_time) *cond(Sampling_period) *cond(Frequency) *cond(Initial_Frequency) *cond(Final_Frequency) *cond(Frequency_step) *cond(Log_frequency_sweep) *cond(Use_transfer_function) "*cond(transfer_function_file)" 
*tcl(GiD_Info layer -bbox -use geometry *layerName)
*cond(Initial_theta) *cond(Final_theta) *cond(Step_theta) *cond(Initial_phi) *cond(Final_phi) *cond(Step_phi)
*end layers
End of Output request instance: 
*end if
End of Output Requests:
