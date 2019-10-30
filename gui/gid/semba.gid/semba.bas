{
    "_format": "Semba Data File in JSON format",
    "_version": "*tcl(set version $semba::VersionNumber)",

*# ----------------------------------------------------------
*# ------------------ SOLVER OPTIONS ------------------------
*# ----------------------------------------------------------
    "solverOptions": {
        "solver": "*GenData(Solver)",
        "runSimulation": *tcl(semba::intToBool *GenData(Run_simulation)),
*if(strcasecmp(GenData(Ending),"Final_time")==0)
        "finalTime": *GenData(Final_time),
*else
        "numberOfTimeSteps": *GenData(Number_of_time_steps),
*endif
        "timeStep": *GenData(Time_step),
        "cfl": *GenData(CFL),
        "defaultSamplingPeriod": *GenData(Default_sampling_period),
        "forceRestarting": *tcl(semba::intToBool *GenData(Force_restarting)),
        "resumeSimulation": *tcl(semba::intToBool *GenData(Resume_simulation)),
        "flush": *GenData(Flush),
        "geometryScalingFactor": *GenData(scaling_factor),
*if(strcmp(GenData(Additional_arguments),"")!=0)
        "additionalArguments": *tcl(json::write string *GenData(Additional_arguments)),
*endif
*# ----
*if(strcasecmp(GenData(Solver),"ugrfdtd")==0)
        "compositesModel": "*GenData(Composites_model)",
        "wiresFlavor": "*GenData(Wires_flavor)",
        "mtln": *tcl(semba::intToBool *GenData(MTLN)),
        "minDistanceWires": *GenData(Min_distance_wires),
        "mapVTK": *tcl(semba::intToBool *GenData(Map_VTK)),
        "mesherOptions": {
            "lowerXBound": "*GenData(Lower_x_bound)",
            "lowerYBound": "*GenData(Lower_y_bound)",
            "lowerZBound": "*GenData(Lower_z_bound)",
            "upperXBound": "*GenData(Upper_x_bound)",
            "upperYBound": "*GenData(Upper_y_bound)",          
            "upperZBound": "*GenData(Upper_z_bound)",
            "vtkExport": *tcl(semba::intToBool *GenData(VTK_Export)),
            "postmshExport": *tcl(semba::intToBool *GenData(postmsh_Export)),
            "mode": "*GenData(Mode)",
            "forbiddenLength": *GenData(Forbidden_length),
            "slantedWires": *tcl(semba::intToBool *GenData(Slanted)),
            "slantedThreshold": *GenData(Segments_filter_threshold)
        }
*elseif(strcasecmp(GenData(Solver),"cudg3d")==0)
        "timeIntegrator": "*GenData(Time_integrator)",
        "useLTS": *GenData(Use_LTS),
        "upwinding": *GenData(Upwinding)
*else
*WarningBox "Unrecognized solver"
        "_error": "invalidSolver"
*endif
    },
    
*# ----------------------------------------------------------
*# -------------------- MATERIALS ---------------------------
*# ----------------------------------------------------------
    "materials": [
*loop materials
        {
            "materialId": *matnum(),
            "name": "*MatProp(0)",
*if(strcmp(MatProp(TypeId),"PEC")==0)
            "materialType": "*MatProp(TypeId)"
*elseif(strcmp(MatProp(TypeId),"PMC")==0)
            "materialType": "*MatProp(TypeId)"
*elseif(strcmp(MatProp(TypeId),"SMA")==0)
            "materialType": "*MatProp(TypeId)"
*elseif(strcmp(MatProp(TypeId),"PML")==0)
            "materialType": "*MatProp(TypeId)",
*if(strcmp(MatProp(Local_Axes),"-GLOBAL-")==0)
            "localAxes": "{0.0 0.0 0.0} {0.0 0.0 0.0}"
*else
            "localAxes": "*tcl(GiD_Info localaxes *matprop(Local_Axes))"
*endif
*elseif(strcmp(Matprop(TypeId),"Classic")==0)
            "materialType": "*MatProp(TypeId)",
            "permittivity": *matprop(Permittivity),
            "permeability": *matprop(Permeability),
            "electricConductivity": *matprop(ElecCond),
            "magneticConductivity": *matprop(MagnCond)
*elseif(strcmp(Matprop(TypeId),"Dispersive")==0)
            "materialType": "*MatProp(TypeId)",
            "filename": *tcl(json::write string *matprop(File))
*elseif(strcmp(Matprop(TypeId),"Wire")==0)
            "materialType": "*MatProp(TypeId)",
            "radius": *matprop(Radius),
            "wireType": "*matprop(WireType)",
*if(strcmp(Matprop(WireType),"Dispersive")==0)
            "filename": *tcl(json::write string *matprop(File))
*elseif(strcmp(Matprop(WireType),"Standard")==0)
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance)
*elseif(strcmp(Matprop(WireType),"SeriesParallel")==0)
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance),
            "capacitance": *matprop(capacitance),   
            "parallelResistance": *matprop(Parallel_Resistance),
            "parallelInductance": *matprop(Parallel_Inductance),
            "parallelCapacitance": *matprop(Parallel_Capacitance)
*else
            "_error": "Invalid wireType"
*endif
*elseif(strcmp(Matprop(TypeId),"Conn_short")==0)
            "materialType": "Connector",
            "connectorType": "*MatProp(TypeId)"
*elseif(strcmp(Matprop(TypeId),"Conn_open")==0)
            "materialType": "Connector",
            "connectorType": "*MatProp(TypeId)"
*elseif(strcmp(Matprop(TypeId),"Conn_sRLC")==0)
            "materialType": "Connector",
            "connectorType": "*MatProp(TypeId)",
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance),
            "capacitance": *matprop(capacitance)
*elseif(strcmp(Matprop(TypeId),"Conn_pRLC")==0)
            "materialType": "Connector",
            "connectorType": "*MatProp(TypeId)",
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance),
            "capacitance": *matprop(capacitance)
*elseif(strcmp(Matprop(TypeId),"Conn_sLpRC")==0)
            "materialType": "Connector",
            "connectorType": "*MatProp(TypeId)",
            "resistance": *matprop(resistance),
            "inductance": *matprop(inductance),
            "capacitance": *matprop(capacitance)
*elseif(strcmp(Matprop(TypeId),"Thin_gap")==0)
            "materialType": "*MatProp(TypeId)",
            "width": *matprop(Width)
*elseif(strcmp(Matprop(TypeId),"Conn_dispersive")==0)
            "materialType": "Connector",
            "connectorType": "*MatProp(TypeId)",
            "filename": *tcl(json::write string *matprop(File))
*elseif(strcmp(Matprop(TypeId),"SIBC")==0)
            "materialType": "*MatProp(TypeId)",
            "surfaceType": "*matprop(SurfaceType)",
*if(strcmp(Matprop(SurfaceType),"File")==0)
            "materialType": "*MatProp(TypeId)",   
            "filename": *tcl(json::write string *matprop(File))
*endif
*if(matprop(Layers,int)==1)
*warningbox "Multilayer did not contain layers"
            "_error": "Multilayer did not contain layers",
*else
*set var num_values(int)=matprop(Layers,int)
            "layers": [
*for(i=1;i<=num_values(int);i=i+4)
                {
                    "thickness":    *matprop(Layers,*i),
                    "permittivity": *matprop(Layers,*operation(i+1)),
                    "permeability": *matprop(Layers,*operation(i+2)),
                    "elecCond":     *matprop(Layers,*operation(i+3))
*if(operation(i+3)!=num_values(int))
                },
*else
                }
*endif
*end for
            ],
*endif
            "freqMin": *matProp(freq_min),
            "freqMax": *matProp(freq_max),
            "numberOfPoles": *matProp(number_Of_Poles),
            "useSembaVectorFitting": *tcl(semba::intToBool *matProp(Use_semba_vector_fitting))
*elseif(strcmp(Matprop(TypeId),"Anisotropic")==0)
            "materialType": "*MatProp(TypeId)"
*if(strcmp(MatProp(Local_Axes),"-GLOBAL-")==0)
            "localAxes": "{0.0 0.0 0.0} {0.0 0.0 0.0}",
*else
            "localAxes": "*tcl([lindex [GiD_Info localaxes *matprop(Local_Axes)] 9])",
*endif
            "anisotropicModel": "*matprop(Anisotropic_model)",
*if(strcmp(MatProp(anisotropicModel),"Crystal")==0)
            "relativePermittivityPrincipalAxes": "*matprop(Relative_permittivity_principal_axes)",
            "crystalRelativePermeability": *matprop(Crystal_relative_permeability)
*elseif(strcmp(MatProp(anisotropicModel),"Ferrite")==0)
            "kappa": *matprop(Kappa),
            "ferriteRelativePermeability": *matprop(Ferrite_relative_permeability),
            "ferriteRelativePermittivity": *matprop(Ferrite_relative_permittivity)
*else
            "_error": "Unrecognized anisotropic model"
*endif
*else
            "_error": "Unrecognized material label: *Matprop(TypeId)"
*warningBox "Unrecognized material label " 
*endif
*if(nmats != loopvar)
        },
*else
        }
*endif
*end materials
    ],

*# ----------------------------------------------------------
*# -------------------- GRIDS -------------------------------
*# ----------------------------------------------------------
    "grids": [
*set var NGRIDS = 0
*set elems(all)
*set Cond Grid
*if(CondNumEntities(int)>0)
*loop layers *OnlyInCond
        {
*set var NGRIDS = NGRIDS + 1
            "gridType": "gridCondition",
            "type": "*cond(Type)",
*if(strcasecmp(cond(Type),"Number_of_cells")==0)
            "numberOfCells": "{*cond(Number_of_cells)}",
*else
            "stepSize": "{*cond(Step_size)}",
*if(strcmp(cond(Fit_size_to_box),"1")==0)
            "fitSizeToBox": true,
*else
            "fitSizeToBox": false,
*endif
*endif   
*if(strcasecmp(cond(boundary_padding_type),"None")==0)      
            "layerBox": "*tcl(GiD_Info layer -bbox -use geometry *layerName)"
*else
            "layerBox": "*tcl(GiD_Info layer -bbox -use geometry *layerName)",
            "directions": "{*cond(Size)}",
            "boundaryPaddingType": "*cond(boundary_padding_type)",
            "upperPadding": "{*cond(Upper_padding)}",
            "lowerPadding": "{*cond(Lower_padding)}",
            "upperPaddingMeshSize": "{*cond(Upper_padding_mesh_size)}",
            "lowerPaddingMeshSize": "{*cond(Lower_padding_mesh_size)}"
*endif
*if(CondNumEntities(int)!=loopVar)
        },
*else
        }
*endif
*end layers
*endif
*if(tcl(expr [lindex [GiD_Cartesian get boxsize] 0] != 0.0))
*if(tcl(expr [lindex [GiD_Cartesian get boxsize] 1] != 0.0))
*if(tcl(expr [lindex [GiD_Cartesian get boxsize] 2] != 0.0))
*set var NGRIDS = NGRIDS + 1
        {
            "gridType": "nativeGiD",
            "corner": "{*tcl(GiD_Cartesian get corner)}",
            "boxSize": "{*tcl(GiD_Cartesian get boxsize)}",
            "nGridPoints": "{*tcl(GiD_Cartesian get ngridpoints)}",
            "xCoordinates": *tcl(semba::getGridCoordinatesAsJSONArrayBAS 0),
            "yCoordinates": *tcl(semba::getGridCoordinatesAsJSONArrayBAS 1),
            "zCoordinates": *tcl(semba::getGridCoordinatesAsJSONArrayBAS 2)
        }
*if(strcasecmp(GenData(Mesher),"DMesheR")==0 && strcasecmp(GenData(Mode),"Structured")!=0)
*if(tcl(expr [GiD_Set MeshType]==1))
*WarningBox "Native GiD meshing can not be used in combination with conformal meshing."
*endif
*endif
*endif
*endif
*endif 
*if(strcasecmp(GenData(Solver),"ugrfdtd")==0)
*if(NGRIDS==0)
*WarningBox "No grids defined. Only one grid is allowed for UGRFDTD"
*endif
*if(NGRIDS>1)
*WarningBox "Multiple grids defined. Only one grid is allowed for UGRFDTD"
*endif

*endif
    ],

*# ----------------------------------------------------------
*# -------------------- LAYERS ------------------------------
*# ----------------------------------------------------------
    "layers": [
*set elems(all)
*loop layers
*set var NUMBER_OF_LAYERS = loopVar
*end layers
*loop layers
*if(NUMBER_OF_LAYERS != loopVar)
        {"id": *LayerNum, "name": "*LayerName"},
*else
        {"id": *LayerNum, "name": "*LayerName"}
*endif
*end layers
    ],

*# ----------------------------------------------------------
*# ----------------- COORDINATES ----------------------------
*# ----------------------------------------------------------
    "coordinates": [
*set elems(all)
*loop nodes
*format "%7i %+14.8e %+14.8e %+14.8e"
*if(npoin != loopVar)
        "*NodesNum *NodesCoord(1,real) *NodesCoord(2,real) *NodesCoord(3,real)",
*else
        "*NodesNum *NodesCoord(1,real) *NodesCoord(2,real) *NodesCoord(3,real)"
*endif
*end nodes
    ],

*# ----------------------------------------------------------
*# ------------------- ELEMENTS -----------------------------
*# ----------------------------------------------------------
    "elements": {
*set elems(Hexahedra)
        "hexahedra": [
*loop elems
*format "%8i %3i %3i %7i %7i %7i %7i %7i %7i %7i %7i"
*if(nelem != loopvar)
        "*ElemsNum *ElemsMat *ElemsLayerNum *ElemsConec",
*else
        "*ElemsNum *ElemsMat *ElemsLayerNum *ElemsConec"
*endif        
*end elems
        ],
*set elems(Tetrahedra)
        "tetrahedra": [
*loop elems
*format "%8i %3i %3i %7i %7i %7i %7i"
*if(nelem != loopvar)
        "*ElemsNum *ElemsMat *ElemsLayerNum *ElemsConec(1) *elemsconec(3) *elemsconec(2) *elemsconec(4)",
*else
        "*ElemsNum *ElemsMat *ElemsLayerNum *ElemsConec(1) *elemsconec(3) *elemsconec(2) *elemsconec(4)"
*endif        
*end elems
        ],
*set elems(Quadrilateral)
        "quadrilateral": [
*loop elems
*format "%8i %3i %3i %7i %7i %7i %7i"
*if(nelem != loopvar)
        "*ElemsNum *ElemsMat *ElemsLayerNum *ElemsConec(1) *elemsconec(2) *elemsconec(3) *elemsconec(4)",
*else
        "*ElemsNum *ElemsMat *ElemsLayerNum *ElemsConec(1) *elemsconec(2) *elemsconec(3) *elemsconec(4)"
*endif
*end elems
        ],
*set elems(Triangle)
        "triangle": [
*loop elems
*format "%8i %3i %3i %7i %7i %7i"
*if(nelem != loopvar)
        "*ElemsNum *ElemsMat *ElemsLayerNum *ElemsConec(1) *elemsconec(2) *elemsconec(3)",
*else
        "*ElemsNum *ElemsMat *ElemsLayerNum *ElemsConec(1) *elemsconec(2) *elemsconec(3)"
*endif
*end elems
        ],
*set elems(Linear)
        "line": [
*loop elems
*format "%8i %3i %3i %7i %7i"
*if(nelem != loopvar)
        "*ElemsNum *ElemsMat *ElemsLayerNum *ElemsConec(1) *ElemsConec(2)",
*else
        "*ElemsNum *ElemsMat *ElemsLayerNum *ElemsConec(1) *ElemsConec(2)"
*endif        
*End elems
        ]
    },

*# ----------------------------------------------------------
*# ------------------ CONNECTOR ON POINT -------------------
*# ----------------------------------------------------------
*# --- Precounts ---
*set var nShort = 0
*set elems(all)
*set Cond Short
*loop nodes *OnlyInCond
*set var nShort = nShort + 1
*end nodes
*set var nOpen = 0
*set Cond Open
*loop nodes *OnlyInCond
*set var nShort = nShort + 1
*end nodes
*set var nRLC = 0
*set Cond RLC
*loop nodes *OnlyInCond
*set var nRLC = nRLC + 1
*end nodes
*set var nConns = nShort + nOpen + nRLC
    "connectorOnPoint": [
*set var nConnsWritten = 0
*set Cond Short
*loop nodes *OnlyInCond
*set var nConnsWritten = nConnsWritten + 1
        {    
            "materialType": "Connector",
            "connectorType": "*cond(TypeId)",
            "coordIds": *NodesNum
*if(nConnsWritten != nConns )
        },
*else
        }
*endif 
*end loop
*set Cond Open
*loop nodes *OnlyInCond
*set var nConnsWritten = nConnsWritten + 1
        {        
            "materialType": "Connector",
            "connectorType": "*cond(TypeId)",
            "coordIds": *NodesNum
*if(nConnsWritten != nConns )
        },
*else
        }
*endif 
*end loop
*set Cond RLC
*loop nodes *OnlyInCond
*set var nConnsWritten = nConnsWritten + 1
        {
            "materialType": "Connector",
            "connectorType": "*cond(TypeId)",
            "resistance": *cond(resistance),
            "inductance": *cond(inductance),
            "capacitance": *cond(capacitance),
            "coordIds": *NodesNum
*if(nConnsWritten != nConns )
        },
*else
        }
*endif 
*end loop
    ],

*# ----------------------------------------------------------
*# ---------------------- SOURCES ---------------------------
*# ----------------------------------------------------------
*# --- Precounts sources ---
*set var nSources = 0
*set elems(all)
*set Cond Planewave
*loop layers *OnlyInCond
*set var nSources = nSources + 1
*end layers
*loop conditions *nodes
*if(strcasecmp(condName,"Generator_on_line")==0)
*loop nodes *OnlyInCond
*set var nSources = nSources + 1
*end nodes
*endif
*end conditions
*Set Cond Source_on_line *bodyElements
*loop elems *OnlyInCond
*set var nSources = nSources + 1
*end elems
*Set Cond Waveguide_port *bodyElements
*loop elems *OnlyInCond
*if(strcasecmp(condName,"Waveguide_port")==0)
*set var nSources = nSources + 1
*endif 
*end elems
*Set Cond TEM_port
*loop elems *OnlyInCond
*if(strcasecmp(condName,"TEM_port")==0)
*set var nSources = nSources + 1
*endif 
*end elems
*# ----------------------------------------------------------
*set var sourceNum = 0
    "sources": [
*set elems(all)
*Set Cond Planewave
*loop layers *OnlyInCond
        {
            "sourceType": "planewave", 
            "definitionMode": "*cond(Definition_mode)",
*if(strcmp(cond(Definition_mode),"by_vectors")==0)
            "directionVector": "{*cond(Direction_vector)}", 
            "polarizationVector": "{*cond(Polarization_vector)}",
*elseif(strcmp(cond(Definition_mode),"by_angles")==0)
            "directionTheta": *cond(Direction_theta),
            "directionPhi": *cond(Direction_phi),
            "polarizationAlpha": *cond(Polarization_alpha),
            "polarizationBeta": *cond(Polarization_beta),
*else
            "numberOfRandomPlanewaves": *cond(Number_of_random_planewaves),
            "relativeVariationOfRandomDelay": *cond(Relative_variation_of_random_delay),
*endif            
*include includes/magnitude.bas
            "layerBox": "*tcl(GiD_Info layer -bbox -use geometry *layerName)"
*set var sourceNum = sourceNum + 1
*#DEBUG     Source number: *sourceNum of *nSources
*if(sourceNum == nSources) 
        }
*else
        },
*endif
*end layers
*# ----------------------------------------------------------
*loop conditions *nodes
*if(strcasecmp(condName,"Generator_on_line")==0)
*loop nodes *OnlyInCond
*set var sourceNum = sourceNum + 1
        {
            "sourceType": "generator",
            "type": "*cond(Type)",
*include includes/magnitude.bas
            "coordIds": [ *NodesNum ]
*if(sourceNum == nSources) 
        }
*else
        },
*endif
*end nodes
*endif
*end conditions
*# ----------------------------------------------------------
*Set Cond Source_on_line *bodyElements
*if(condNumEntities!=0)
*loop elems *OnlyInCond
*set var sourceNum = sourceNum + 1
*end elems
*tcl(semba::writeSourceOnLineBAS)
*endif
*# ----------------------------------------------------------
*set cond Waveguide_port *bodyElements
*if(strcasecmp(condName,"Waveguide_port")==0&&condNumEntities>0)
*set var HEADER = 0
*loop elems *onlyInCond
*if(HEADER == 0)
*set var HEADER = 1
        {
            "sourceType": "waveguidePort",
            "shape": "*cond(Shape)",
*include includes/magnitude.bas
            "excitationMode": "*cond(Mode)",
            "firstMode":  *cond(FirstMode),
            "secondMode": *cond(SecondMode),
            "elemIds": [
*endif
*if(loopVar != condNumEntities)
                *ElemsNum,
*else
                *ElemsNum
*endif
*set var sourceNum = sourceNum + 1
*end elems
            ]
*if(sourceNum == nSources) 
        }
*else
        },
*endif
*endif 
*# ----------------------------------------------------------
*set cond TEM_port *bodyElements
*if(strcasecmp(condName,"TEM_port")==0&&condNumEntities>0)
*set var ENTITY = 0
*loop elems *onlyInCond
*if(ENTITY == 0)
*set var ENTITY = cond(entityId,INT)
        {
            "sourceType": "temPort",
*include includes/magnitude.bas
            "origin": "*tcl(GiD_Info Coordinates *cond(Origin) geometry)",
            "innerRadius": *cond(Inner_radius),
            "outerRadius": *cond(Outer_radius),
            "excitationMode": "*cond(Mode)",
            "elemIds": [
*endif
*if(loopVar != condNumEntities)
                *ElemsNum,
*else
                *ElemsNum
*endif
*set var sourceNum = sourceNum + 1
*end elems
            ]
*if(sourceNum == nSources) 
        }
*else
        },
*endif
*endif 
    ],
*# ----------------------------------------------------------
*# ------------------ OUTPUT REQUESTS -----------------------
*# ----------------------------------------------------------
*# --- Precounts output requests ----------------------------
*tcl(semba::countAllElementsInOutputRequestsBAS)
*# --- Writes output requests -------------------------------
    "outputRequests": [
*# ----------------------------------------------------------
*tcl(semba::writeOutputRequestBAS OutRq_on_point)
*# ----------------------------------------------------------
*tcl(semba::writeOutputRequestBAS OutRq_on_line)
*# ----------------------------------------------------------
*tcl(semba::writeOutputRequestBAS OutRq_on_surface)
*# ----------------------------------------------------------
*Set cond OutRq_on_layer
*if(CondNumEntities(int)>0)
*loop layers *OnlyInCond
        {
            "gidOutputType": "OutRq_on_layer",
            "name": "*cond(Name)",
            "type": "*cond(Type)",
*if(strcasecmp(Cond(Type),"bulkCurrentElectric")==0 || strcasecmp(Cond(Type),"bulkCurrentMagnetic"))
            "skip": *cond(Skip),
            "direction": "*cond(Direction)",
*endif
*include includes/domain.bas
            "box": "*tcl(GiD_Info layer -bbox -use geometry *layerName)"
        }*tcl(semba::printCommaIfPendingOutRq)
*end layers
*end if
*# ----------------------------------------------------------
*Set cond Far_field
*if(CondNumEntities(int)>0)
        { 
            "gidOutputType": "Far_field",
*loop layers *OnlyInCond
            "name": "*cond(Name)",
            "type": "*cond(Type)",
*include includes/domain.bas
            "box": "*tcl(GiD_Info layer -bbox -use geometry *layerName)",
            "initialTheta": *cond(Initial_theta),
            "finalTheta":   *cond(Final_theta),
            "stepTheta":    *cond(Step_theta),
            "initialPhi":   *cond(Initial_phi),
            "finalPhi":     *cond(Final_phi),
            "stepPhi":      *cond(Step_phi)
*end layers
        }*tcl(semba::printCommaIfPendingOutRq)
*end if
    ]
*# ----------------------------------------------------------
}