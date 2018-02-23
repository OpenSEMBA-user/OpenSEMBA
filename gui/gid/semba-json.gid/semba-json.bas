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
            "bruteForceVolumes": *GenData(Brute_force_volumes),
            "vtkExport": *GenData(VTK_Export),
            "postmshExport": *GenData(postmsh_Export),
            "mode": "*GenData(Mode)",
            "forbiddenLength": *GenData(Forbidden_length),
            "slantedWires": *GenData(Slanted),
            "slantedThreshold": *GenData(Segments_filter_threshold)
        }
*elseif(strcasecmp(GenData(Solver),"cudg3d")==0)
        "timeIntegrator": *GenData(Time_integrator),
        "useLTS": *GenData(Use_LTS),
        "upwinding": *GenData(Upwinding)
*else
*WarningBox Unrecognized solver
*endif
    },
     
    "problemSize": {
*set elems(Hexahedra)
        "hexahedra8": *nelem,
*set elems(Tetrahedra)
        "tetrahedra4": *nelem,
*set elems(Quadrilateral)
        "quadrilateral4": *nelem,
*set elems(Triangle)
        "triangle3": *nelem,
*set elems(Linear)
        "linear2": *nelem,
*set elems(all)
        "coordinates": *npoin,
        "materials": *nmats
    },

    "materials": {
*loop materials
        "material": {
            "materialId": *matnum(),
            "name": "*MatProp(0)",
            "materialTypeId": "*MatProp(TypeId)",
*if(strcmp(MatProp(TypeId),"PEC")==0)
*endif
*if(strcmp(MatProp(TypeId),"PMC")==0)
*endif
*if(strcmp(MatProp(TypeId),"PML")==0)
            "automaticOrientation": *matprop(Automatic_orientation),
*if(strcmp(MatProp(Local_Axes),"-GLOBAL-")==0)
            "localAxes": "{0.0 0.0 0.0} {0.0 0.0 0.0}",
*else
            "localAxes: "*tcl(GiD_Info localaxes *matprop(Local_Axes))",
*endif
*endif
*if(strcmp(Matprop(TypeId),"Classic")==0)
            "permittivity": *matprop(Permittivity),
            "permeability": *matprop(Permeability),
            "electricConductivity": *matprop(ElecCond),
            "magneticConductivity": *matprop(MagnCond),
*endif
*if(strcmp(Matprop(TypeId),"Dispersive")==0)
            "filename": "*matprop(File)",
*endif
*if(strcmp(Matprop(TypeId),"Wire")==0)
            "radius": *matprop(Radius),
            "wireType": "*matprop(WireType)",
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance),
            "capacitance": *matprop(capacitance),   
            "parallelResistance": *matprop(Parallel_Resistance),
            "parallelInductance": *matprop(Parallel_Inductance),
            "parallelCapacitance": *matprop(Parallel_Capacitance),
*if(strcmp(Matprop(WireType),"Dispersive")==0)
            "filename": *matprop(File),
*endif
*endif
*if(strcmp(Matprop(TypeId),"Conn_sRLC")==0)
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance),
            "capacitance": *matprop(capacitance),
*endif
*if(strcmp(Matprop(TypeId),"Conn_pRLC")==0)
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance),
            "capacitance": *matprop(capacitance),
*endif
*if(strcmp(Matprop(TypeId),"Conn_sLpRC")==0)
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance),
            "capacitance": *matprop(capacitance),
*endif
*if(strcmp(Matprop(TypeId),"Thin_gap")==0)
            "width": *matprop(Width),
*endif
*if(strcmp(Matprop(TypeId),"Conn_dispersive")==0)
            "filename": "*matprop(File)",
*endif
*if(strcmp(Matprop(TypeId),"SIBC")==0)
            "surfaceType": "*matprop(SurfaceType)",
*if(strcmp(Matprop(SurfaceType),"File")==0)
            "filename": "*matprop(File)"
*endif
            "layers": "*matprop",
*endif
*if(strcmp(Matprop(TypeId),"Anisotropic")==0)
*if(strcmp(MatProp(Local_Axes),"-GLOBAL-")==0)
            "localAxes": "{0.0 0.0 0.0} {0.0 0.0 0.0}",
*else
            "localAxes": "*tcl(GiD_Info localaxes *matprop(Local_Axes))",
*endif
            "anisotropicModel": "*matprop(Anisotropic_model)",
            "relativePermittivityPrincipalAxes": "*matprop(Relative_permittivity_principal_axes)",
            "crystalRelativePermeability": *matprop(Crystal_relative_permeability),
            "kappa": *matprop(Kappa),
            "ferriteRelativePermeability": *matprop(Ferrite_relative_permeability),
            "ferriteRelativePermittivity": *matprop(Ferrite_relative_permittivity)
*endif
        }
*end materials
    },

    "grids": { 
*if(strcasecmp(GenData(Mesher),"None")!=0)
*set elems(all)
*set Cond Grid
*if(CondNumEntities(int)>0)
*loop layers *OnlyInCond
        "gridCondition": {
            "layerBox": "*tcl(GiD_Info layer -bbox -use geometry *layerName)",
            "type": "*cond(Type)",
            "directions": "{*cond(x_direction) *cond(y_direction) *cond(z_direction)}",
            "boundaryPaddingType": "*cond(boundary_padding_type)",
            "boundaryPadding": "{*cond(Upper_x_boundary_padding) *cond(Upper_y_boundary_padding) *cond(Upper_z_boundary_padding) *cond(Lower_x_boundary_padding) *cond(Lower_y_boundary_padding) *cond(Lower_z_boundary_padding)}",
            "boundaryMeshSize": "{*cond(Upper_x_boundary_mesh_size) *cond(Upper_y_boundary_mesh_size) *cond(Upper_z_boundary_mesh_size) *cond(Lower_x_boundary_mesh_size) *cond(Lower_y_boundary_mesh_size) *cond(Lower_z_boundary_mesh_size)}"
        }
*end layers
*endif
*elseif(tcl(expr [GiD_Cartesian get dimension] != -1))
        "nativeGiD": {
            "corner": "*tcl(GiD_Cartesian get corner)",
            "boxSize": "*tcl(GiD_Cartesian get boxsize)",
            "nGridPoints": "{*tcl(GiD_Cartesian get ngridpoints)}",
            "coordinates": "*tcl(GiD_Cartesian get coordinates)"
        }
*else 
*if(strcasecmp(GenData(Solver),"ugrfdtd")==0)
*WarningBox "No grid defined"
*endif
*endif
    },

    "layers": {
*set elems(all)
*loop layers
        "layer": {"id": *LayerNum, "name": "*LayerName"}
*end layers
    }

    "coordinates": [
*set elems(all)
*loop nodes
*format "%7i %+14.8e %+14.8e %+14.8e"
        *NodesNum *NodesCoord(1,real) *NodesCoord(2,real) *NodesCoord(3,real)
*end nodes
    ]

    "elements": {
*set elems(Hexahedra)
        "hexahedra": [
*loop elems
*format "%8i %7i %7i %7i %7i %7i %7i %7i %7i %3i %3i"
        *ElemsNum *ElemsConec *ElemsMat *ElemsLayerNum 
*end elems
        ],
*set elems(Tetrahedra)
        "tetrahedra": [
*loop elems
*format "%8i %7i %7i %7i %7i %3i %3i"
        *ElemsNum *ElemsConec(1) *elemsconec(3) *elemsconec(2) *elemsconec(4) *ElemsMat *ElemsLayerNum
*end elems
        ],
*set elems(Quadrilateral)
        "quadrilateral": [
*loop elems
*format "%8i %7i %7i %7i %7i %3i %3i"
        *ElemsNum *ElemsConec(1) *elemsconec(2) *elemsconec(3) *elemsconec(4) *ElemsMat *ElemsLayerNum 
*end elems
        ],
*set elems(Triangle)
        "triangle": [
*loop elems
*format "%8i %7i %7i %7i %3i %3i"
        *ElemsNum *ElemsConec(1) *elemsconec(2) *elemsconec(3) *ElemsMat *ElemsLayerNum 
*end elems
        ],
*set elems(Linear)
        "line": [
*loop elems
*format "%8i %7i %7i %3i %3i"
        *ElemsNum *ElemsConec(1) *ElemsConec(2) *ElemsMat *ElemsLayerNum
*End elems
        ]
    }

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