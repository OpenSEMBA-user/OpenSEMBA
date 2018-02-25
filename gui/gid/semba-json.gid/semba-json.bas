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
        "wiresFlavor": "*GenData(Wires_flavor)",
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
        "_error": "invalidSolver"
*endif
    },
     
    "problemSize": {
*set elems(Hexahedra)
        "hexahedra8":     *nelem,
*set elems(Tetrahedra)
        "tetrahedra4":    *nelem,
*set elems(Quadrilateral)
        "quadrilateral4": *nelem,
*set elems(Triangle)
        "triangle3":      *nelem,
*set elems(Linear)
        "linear2":        *nelem,
*set elems(all)
        "coordinates":    *npoin,
        "materials":      *nmats
    },

    "materials": [
*loop materials
        {
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
        },
*end materials
    ],

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
        },
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
*WarningBox "No grid defined. Define grid condition or use GiD native mesher."
        "_error": "gridError"
*endif
*endif
    },

    "layers": [
*set elems(all)
*loop layers
        {"id": *LayerNum, "name": "*LayerName"},
*end layers
    ]

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
    },

    "sources": [
*set elems(all)
*Set Cond Planewave
*if(CondNumEntities(int)>0)
# ------------------------
        {
            "sourceType": "planewave", 
*loop layers *OnlyInCond
            "definitionMode": "*cond(Definition_mode)",
*if(strcmp(cond(Definition_mode),"by_vectors")==0)
            "directionVector"   : "{*cond(Direction_vector)}", 
            "polarizationVector": "{*cond(Polarization_vector)}",
*elseif(strcmp(cond(Definition_mode),"by_angles")==0)
            "directionTheta"    : *cond(Direction_theta),
            "directionPhi"      : *cond(Direction_phi),
            "polarizationAlpha" : *cond(Polarization_alpha),
            "polarizationBeta"  : *cond(Polarization_beta),
*else
            "numberOfRandomPlanewaves": *cond(Number_of_random_planewaves),
            "relativeVariationOfRandomDelay": *cond(Relative_variation_of_random_delay),
*endif            
            "magnitude": {
                "type": "*cond(magnitude)",
*if(strcmp(cond(Magnitude),"File")==0)
                "filename": "*cond(file)"
*else
                "gaussianSpread": *cond(Gaussian_spread),
                "gaussianDelay": *cond(Gaussian_delay)
*endif
            },
            "defined": "OnLayers",
            "layerName": "*layerName",
            "layerBox": "*tcl(GiD_Info layer -bbox -use geometry *layerName)"
*end layers
        },
*endif
# ------------------------
*Set Cond Source_on_line *bodyElements
*set var HEADER=0
*loop elems *OnlyInCond
*if(HEADER==0) 
*set var HEADER=1
        {
            "sourceType": "sourceOnLine",
            "type": "*cond(Type)", 
            "hardness": "*cond(Hardness)",
            "magnitude": {
                "type": "*cond(magnitude)",
*if(strcmp(cond(Magnitude),"File")==0)
                "filename": "*cond(file)"
*else
                "gaussianSpread": *cond(Gaussian_spread),
                "gaussianDelay": *cond(Gaussian_delay)
*endif
            },
            "defined": "OnElements",
            "numberOfElements": *CondNumEntities(int),
            "elemIds": [
*endif
                *elemsNum
*end elems
            ]
*if(HEADER==1)
}
*endif
# ------------------------
*loop conditions *nodes
*if(strcasecmp(condName,"Generator_on_line")==0&&CondNumEntities(int)>0)
*loop nodes *OnlyInCond
        {
            "sourceType": "generator",
            "type": "*cond(Type)",
            "magnitude": {
                "type": "*cond(magnitude)",
            *if(strcmp(cond(Magnitude),"File")==0)
                "filename": "*cond(file)"
            *else
                "gaussianSpread": *cond(Gaussian_spread),
                "gaussianDelay": *cond(Gaussian_delay)
            *endif
            },
            "defined": "OnNodes",
            "numberOfElements": 1,
            "elemIds": [ *NodesNum ]
        },
*end nodes
*endif
*end conditions
# ------------------------
*loop conditions *bodyElements
*if(strcasecmp(condName,"Waveguide_port")==0&&condNumEntities>0)
*set var HEADER = 0
*loop elems *onlyInCond
*if(HEADER == 0)
        {
            "sourceType": "waveguidePort",
            "shape": "*cond(Shape)"
            "magnitude": {
                "type": "*cond(magnitude)",
            *if(strcmp(cond(Magnitude),"File")==0)
                "filename": "*cond(file)"
            *else
                "gaussianSpread": *cond(Gaussian_spread),
                "gaussianDelay": *cond(Gaussian_delay)
            *endif
            },
            "excitationMode": "*cond(Mode)",
            "firstMode": *cond(FirstMode),
            "secondMode": *cond(SecondMode),
            "numberOfElements": *CondNumEntities(int),
            "elemIds": [
*set var HEADER = 1
*endif
                *ElemsNum
*end elems
            ]
        },
*endif 
*end conditions

*loop conditions *faceElements
*if(strcasecmp(condName,"TEM_port")==0&&condNumEntities>0)
*set var HEADER = 0
*loop elems *onlyInCond
*if(HEADER == 0)
        {
            "sourceType": "temPort",
            "magnitude": {
                "type": "*cond(magnitude)",
            *if(strcmp(cond(Magnitude),"File")==0)
                "filename": "*cond(file)"
            *else
                "gaussianSpread": *cond(Gaussian_spread),
                "gaussianDelay": *cond(Gaussian_delay)
            *endif
            },
            "origin": "{*cond(Origin)}",
            "innerRadius": *cond(Inner_radius),
            "outerRadius": *cond(Outer_radius),
            "excitationMode": "*cond(Mode)",
            "numberOfElements": *CondNumEntities(int)
            "elemIds": [
*set var HEADER = 1
*endif
                *ElemsNum,
*end elems
            ]
        },
*endif 
*end conditions
    ],

# ================================================
    "outputRequests": {
*Set Cond OutRq_on_point
*if(CondNumEntities(int)>0)
        {
            "gidOutputType": "OutRq_on_point",
            "numberOfElements": *CondNumEntities(int),
*loop nodes *OnlyInCond
            "name": "*cond(Name)",
            "type": "*cond(Type)",
            "domain": {
*if(strcmp(cond(Time),"1")==0)
                "time": "*cond(Time)",
                "initialTime": *cond(Initial_time),
                "finalTime": *cond(Final_time),
                "samplingPeriod": *cond(Sampling_period),
*endif
*if(strcmp(cond(Frequency),"1")==0)
                "frequency": *cond(Frequency),
                "initialFrequency": *cond(Initial_Frequency),
                "finalFrequency": *cond(Final_Frequency),
                "frequencyStep": *cond(Frequency_step),
                "logFrequencySweep": *cond(Log_frequency_sweep),
                "useTransferFunction": *cond(Use_transfer_function),
                "transferFunctionFile": "*cond(transfer_function_file)"
*endif
            },                
            "CoordId": *NodesNum
*end nodes
        },
*endif
# -----------------------------------
*Set cond OutRq_on_line
*if(CondNumEntities(int)>0)
        {
            "gidOutputType": "OutRq_on_line",
            "numberOfElements": *CondNumEntities(int),
*loop elems OnlyInCond
            "name": *cond(Name),
            "type": *cond(Type),
            "domain": {
*if(strcmp(cond(Time),"1")==0)
                "time": "*cond(Time)",
                "initialTime": *cond(Initial_time),
                "finalTime": *cond(Final_time),
                "samplingPeriod": *cond(Sampling_period),
*endif
*if(strcmp(cond(Frequency),"1")==0)
                "frequency": *cond(Frequency),
                "initialFrequency": *cond(Initial_Frequency),
                "finalFrequency": *cond(Final_Frequency),
                "frequencyStep": *cond(Frequency_step),
                "logFrequencySweep": *cond(Log_frequency_sweep),
                "useTransferFunction": *cond(Use_transfer_function),
                "transferFunctionFile": "*cond(transfer_function_file)"
*endif
            },                
            "elemId": *elemsNum
*end elems
        },
*end if
# -----------------------------------
*Set cond OutRq_on_surface
*if(CondNumEntities(int)>0)
        {
            "gidOutputType": "OutRq_on_surface",
            "numberOfElements": *CondNumEntities(int),
*loop elems OnlyInCond
            "name": *cond(Name),
            "type": *cond(Type),
            "domain": {
*if(strcmp(cond(Time),"1")==0)
                "time": "*cond(Time)",
                "initialTime": *cond(Initial_time),
                "finalTime": *cond(Final_time),
                "samplingPeriod": *cond(Sampling_period),
*endif
*if(strcmp(cond(Frequency),"1")==0)
                "frequency": *cond(Frequency),
                "initialFrequency": *cond(Initial_Frequency),
                "finalFrequency": *cond(Final_Frequency),
                "frequencyStep": *cond(Frequency_step),
                "logFrequencySweep": *cond(Log_frequency_sweep),
                "useTransferFunction": *cond(Use_transfer_function),
                "transferFunctionFile": "*cond(transfer_function_file)"
*endif
            },                
            "elemId": *elemsNum
*end elems
        },
*end if
# -----------------------------------
*Set cond OutRq_on_layer
*if(CondNumEntities(int)>0)
        {
            "gidOutputType": "OutRq_on_layer",
            "numberOfElements": *CondNumEntities(int),
*loop layers *OnlyInCond
            "name": *cond(Name),
            "type": *cond(Type),
            "domain": {
*if(strcmp(cond(Time),"1")==0)
                "time": "*cond(Time)",
                "initialTime": *cond(Initial_time),
                "finalTime": *cond(Final_time),
                "samplingPeriod": *cond(Sampling_period),
*endif
*if(strcmp(cond(Frequency),"1")==0)
                "frequency": *cond(Frequency),
                "initialFrequency": *cond(Initial_Frequency),
                "finalFrequency": *cond(Final_Frequency),
                "frequencyStep": *cond(Frequency_step),
                "logFrequencySweep": *cond(Log_frequency_sweep),
                "useTransferFunction": *cond(Use_transfer_function),
                "transferFunctionFile": "*cond(transfer_function_file)"
*endif
            },                
            "box": "{*tcl(GiD_Info layer -bbox -use geometry *layerName)}"
*end layers
        },
*end if
# -----------------------------------
*Set cond Bulk_current_on_surface
*if(CondNumEntities(int)>0)
        {
            "gidOutputType": "Bulk_current_on_surface",
            "numberOfElements": *CondNumEntities(int),
*loop elems *OnlyInCond
            "name": *cond(Name),
            "type": *cond(Type),
            "domain": {
*if(strcmp(cond(Time),"1")==0)
                "time": "*cond(Time)",
                "initialTime": *cond(Initial_time),
                "finalTime": *cond(Final_time),
                "samplingPeriod": *cond(Sampling_period),
*endif
*if(strcmp(cond(Frequency),"1")==0)
                "frequency": *cond(Frequency),
                "initialFrequency": *cond(Initial_Frequency),
                "finalFrequency": *cond(Final_Frequency),
                "frequencyStep": *cond(Frequency_step),
                "logFrequencySweep": *cond(Log_frequency_sweep),
                "useTransferFunction": *cond(Use_transfer_function),
                "transferFunctionFile": "*cond(transfer_function_file)"
*endif
            },                
            "direction": "{*cond(Direction)}",
            "skip": *cond(Skip),
            "elemId": *elemsNum
*end elems
        },
*end if
# -----------------------------------
*Set cond Bulk_current_on_layer
*if(CondNumEntities(int)>0)
        {
            "gidOutputType": "Bulk_current_on_layer",
            "numberOfElements": *CondNumEntities(int),
*loop layers *OnlyInCond
            "name": *cond(Name),
            "type": *cond(Type),
            "domain": {
*if(strcmp(cond(Time),"1")==0)
                "time": "*cond(Time)",
                "initialTime": *cond(Initial_time),
                "finalTime": *cond(Final_time),
                "samplingPeriod": *cond(Sampling_period),
*endif
*if(strcmp(cond(Frequency),"1")==0)
                "frequency": *cond(Frequency),
                "initialFrequency": *cond(Initial_Frequency),
                "finalFrequency": *cond(Final_Frequency),
                "frequencyStep": *cond(Frequency_step),
                "logFrequencySweep": *cond(Log_frequency_sweep),
                "useTransferFunction": *cond(Use_transfer_function),
                "transferFunctionFile": "*cond(transfer_function_file)"
*endif
            },                
            "direction": "{*cond(Direction)}",
            "skip": *cond(Skip),
            "box": {*tcl(GiD_Info layer -bbox -use geometry *layerName)}
*end layers
        },
*end if
# -----------------------------------
*Set cond Far_field
*if(CondNumEntities(int)>0)
        { 
            "gidOutputType": "Far_field",
            "numberOfElements": *CondNumEntities(int),
*loop layers *OnlyInCond
            "name": *cond(Name),
            "type": *cond(Type),
            "domain": {
*if(strcmp(cond(Time),"1")==0)
                "time": "*cond(Time)",
                "initialTime": *cond(Initial_time),
                "finalTime": *cond(Final_time),
                "samplingPeriod": *cond(Sampling_period),
*endif
*if(strcmp(cond(Frequency),"1")==0)
                "frequency": *cond(Frequency),
                "initialFrequency": *cond(Initial_Frequency),
                "finalFrequency": *cond(Final_Frequency),
                "frequencyStep": *cond(Frequency_step),
                "logFrequencySweep": *cond(Log_frequency_sweep),
                "useTransferFunction": *cond(Use_transfer_function),
                "transferFunctionFile": "*cond(transfer_function_file)"
*endif
            },                
            "box": "{*tcl(GiD_Info layer -bbox -use geometry *layerName)}",
            "farPoints": {
                "initialTheta": *cond(Initial_theta),
                "finalTheta": *cond(Final_theta),
                "stepTheta":*cond(Step_theta),
                "initialPhi": *cond(Initial_phi),
                "finalPhi": *cond(Final_phi),
                "stepPhi": *cond(Step_phi)
            }
*end layers
        }
*end if
    }
}