{
    "format": "Semba Data File",
    "version": "*tcl(set version $semba::VersionNumber)",

    "solverOptions": {
        "solverName": "*GenData(Solver)",
        "runSimulation": *GenData(Run_simulation),
*if(strcasecmp(GenData(Ending),"Final_time")==0)
        "finalTime": *GenData(Final_time),
*else
        "numberOfTimeSteps": *GenData(Number_of_time_steps),
*endif
        "timeStep": *GenData(Time_step),
        "cfl": *GenData(CFL),
        "defaultSamplingPeriod": *GenData(Default_sampling_period),
        "forceRestarting": *GenData(Force_restarting),
        "resumeSimulation": *GenData(Resume_simulation),
        "flush": *GenData(Flush),
*if(strcmp(GenData(Additional_arguments),"")!=0)
        "additionalArguments": *GenData(Additional_arguments),
*endif

*if(strcasecmp(GenData(Solver),"ugrfdtd")==0)
        "compositesModel": "*GenData(Composites_model)",
        "wiresFlavor: "*GenData(Wires_flavor)",
        "mtln": *GenData(MTLN),
        "minDistanceWires": *GenData(Min_distance_wires),
        "mapVTK": *GenData(Map_VTK),
        "geometryScalingFactor": *GenData(scaling_factor),

        "mesherOptions": {
            "upperXBound": "*GenData(Upper_x_bound)",
            "lowerXBound": "*GenData(Lower_x_bound)",
            "upperYBound": "*GenData(Upper_y_bound)",
            "lowerYBound": "*GenData(Lower_y_bound)",
            "upperZBound": "*GenData(Upper_z_bound)",
            "lowerZBound": "*GenData(Lower_z_bound)",
            "mesher": "*GenData(Mesher)",
            Brute force volumes: *GenData(Brute_force_volumes)
            VTK Export: *GenData(VTK_Export)
            postmsh Export: *GenData(postmsh_Export)
            Mode: *GenData(Mode)
            Forbidden length: *GenData(Forbidden_length)
            Slanted wires: *GenData(Slanted)
            Slanted threshold: *GenData(Segments_filter_threshold)
        }
*elseif(strcasecmp(GenData(Solver),"cudg3d")==0)
        "timeIntegrator": *GenData(Time_integrator),
        "useLTS": *GenData(Use_LTS),
        "upwinding": *GenData(Upwinding)
*else
*WarningBox Unrecognized solver
*endif
    }
    
    
Problem size:
*set elems(Hexahedra)
Hexahedral elements: *nelem
*set elems(Tetrahedra)
Tetrahedral elements: *nelem
*set elems(Quadrilateral)
Quadrilateral elements: *nelem
*set elems(Triangle)
Triangular elements: *nelem
*set elems(Linear)
Line elements: *nelem
*set elems(all)
Number of points: *npoin
Number of materials: *nmats
End of Problem size:

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
WireType: *matprop(WireType)
Resistance: *matprop(Resistance)
Inductance: *matprop(Inductance)
Capacitance: *matprop(Capacitance)
Parallel Resistance: *matprop(Parallel_Resistance)
Parallel Inductance: *matprop(Parallel_Inductance)
Parallel Capacitance: *matprop(Parallel_Capacitance)
*if(strcmp(Matprop(WireType),"Dispersive")==0)
Filename: *matprop(File)
*endif
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
*if(strcmp(Matprop(TypeId),"Thin_gap")==0)
Width: *matprop(Width)
*endif
*if(strcmp(Matprop(TypeId),"Conn_dispersive")==0)
Filename: *matprop(File)
*endif
*if(strcmp(Matprop(TypeId),"SIBC")==0)
SurfaceType: *matprop(SurfaceType)
*if(strcmp(Matprop(SurfaceType),"File")==0)
Filename: *matprop(File)
*endif
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
End of Materials:

Grid:
*if(strcasecmp(GenData(Mesher),"None")!=0)
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
*elseif(tcl(expr [GiD_Cartesian get dimension] != -1))
GiD Native Cartesian Mesher: *tcl(GiD_Cartesian get iscartesian)
Corner:       *tcl(GiD_Cartesian get corner) 
BoxSize:      *tcl(GiD_Cartesian get boxsize)
NGridPoints:  *tcl(GiD_Cartesian get ngridpoints) 
Dimension:    *tcl(GiD_Cartesian get dimension)
Coordinates:  *tcl(GiD_Cartesian get coordinates)
*else 
*if(strcasecmp(GenData(Solver),"ugrfdtd")==0)
*WarningBox "No grid defined"
*endif
*endif
End of Grid:

Layers:
*set elems(all)
*loop layers
*LayerNum *LayerName
*end layers
End of Layers:

Coordinates:
*set elems(all)
*loop nodes
*format "%7i %+14.8e %+14.8e %+14.8e"
*NodesNum *NodesCoord(1,real) *NodesCoord(2,real) *NodesCoord(3,real)
*end nodes
End of Coordinates:

Elements:
*set elems(Hexahedra)
Hexahedral Elements:
*loop elems
*format "%8i %7i %7i %7i %7i %7i %7i %7i %7i %3i %3i"
*ElemsNum *ElemsConec *ElemsMat *ElemsLayerNum 
*end elems
End of Hexahedral Elements:
*set elems(Tetrahedra)
Tetrahedral Elements:
*loop elems
*format "%8i %7i %7i %7i %7i %3i %3i"
*ElemsNum *ElemsConec(1) *elemsconec(3) *elemsconec(2) *elemsconec(4) *ElemsMat *ElemsLayerNum
*end elems
End of Tetrahedral Elements:
*set elems(Quadrilateral)
Quadrilateral Elements:
*loop elems
*format "%8i %7i %7i %7i %7i %3i %3i"
*ElemsNum *ElemsConec(1) *elemsconec(2) *elemsconec(3) *elemsconec(4) *ElemsMat *ElemsLayerNum 
*end elems
End of Quadrilateral Elements:
*set elems(Triangle)
Triangle Elements:
*loop elems
*format "%8i %7i %7i %7i %3i %3i"
*ElemsNum *ElemsConec(1) *elemsconec(2) *elemsconec(3) *ElemsMat *ElemsLayerNum 
*end elems
End of Triangle Elements:
*set elems(Linear)
Line Elements:
*loop elems
*format "%8i %7i %7i %3i %3i"
*ElemsNum *ElemsConec(1) *ElemsConec(2) *ElemsMat *ElemsLayerNum
*End elems
End of Line Elements:
End of Elements:

Excitations:
*set elems(all)
*Set Cond Planewave
*if(CondNumEntities(int)>0)
Planewave: 
*loop layers *OnlyInCond
Definition mode: *cond(Definition_mode)
Direction vector: *cond(Direction_vector) 
Polarization vector: *cond(Polarization_vector)
Direction theta: *cond(Direction_theta)
Direction phi: *cond(Direction_phi)
Polarization alpha: *cond(Polarization_alpha)
Polarization beta: *cond(Polarization_beta)
Number of random planewaves: *cond(Number_of_random_planewaves)
Relative variation of random delay: *cond(Relative_variation_of_random_delay)
Excitation: *cond(Excitation)
Gaussian spread: *cond(Gaussian_spread)
Gaussian delay: *cond(Gaussian_delay)
*if(strcmp(cond(Excitation),"File")==0)
Filename: *cond(File)
*endif
Defined: OnLayers
Layer Name: *layerName
Layer Box: *tcl(GiD_Info layer -bbox -use geometry *layerName)
*end layers
End of Planewave:
*endif

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
*if(strcmp(cond(Excitation),"File")==0)
 Filename: *cond(File)
*endif
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
*if(strcmp(cond(Excitation),"File")==0)
 Filename: *cond(File)
*endif
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
 Shape: *cond(Shape)
 Excitation: *cond(Excitation)
 Gaussian spread: *cond(Gaussian_spread)
 Gaussian delay: *cond(Gaussian_delay)
*if(strcmp(cond(Excitation),"File")==0)
 Filename: *cond(File)
*endif
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
 Shape: *cond(Shape)
 Excitation: *cond(Excitation)
 Gaussian spread: *cond(Gaussian_spread)
 Gaussian delay: *cond(Gaussian_delay)
*if(strcmp(cond(Excitation),"File")==0)
 Filename: *cond(File)
*endif
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
*if(strcmp(cond(Excitation),"File")==0)
 Filename: *cond(File)
*endif
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
*if(strcmp(cond(Excitation),"File")==0)
 Filename: *cond(File)
*endif
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
*Set cond OutRq_on_line
*if(CondNumEntities(int)>0)
Output request instance: 
GiDOutputType: OutRq_on_line
Number of elements: *CondNumEntities(int)
*loop elems OnlyInCond
Name: *cond(Name) 
Type: *cond(Type) 
Domain: *cond(Time) *cond(Initial_time) *cond(Final_time) *cond(Sampling_period) *cond(Frequency) *cond(Initial_Frequency) *cond(Final_Frequency) *cond(Frequency_step) *cond(Log_frequency_sweep) *cond(Use_transfer_function) "*cond(transfer_function_file)" 
*elemsNum
*end elems
End of Output request instance:
*end if
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
*Set cond OutRq_on_layer
*if(CondNumEntities(int)>0)
Output request instance: 
GiDOutputType: OutRq_on_layer
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
*Set cond Bulk_current_on_layer
*if(CondNumEntities(int)>0)
Output request instance: 
GiDOutputType: Bulk_current_on_layer
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
}