var config = {
            dataSource: 'dcg.json',
            directedEdges: true,
            edgeTypes: {"edgeType":["CALL", "READ", "WRITE"]}, 
            nodeTypes: {"nodeType":["Function", "Variable"]},
            forceLocked: true,
            nodeCaption: "caption",
            edgeCaption: "edgeType",
            nodeStyle: {
              "Function": {
                "color"      : "rgb(255,255,0)",
                "radius"     : 20,
                "borderWidth": 8
              },
              "Variable":{
                "color"      : "rgb(55,255,152)",
                "radius"     : 4
              }
            },
            edgeStyle: {
              "CALL": {
                "width": 1,
                "color": "rgba(255,255,255,0.4)"
              },
              "READ": {
                "width": 3,
                "color": "rgb(0,255,0)"
              },
              "WRITE": {
                "width": 5,
                "color": "rgb(0,255,0)"
              }
            },
            showControlDash: true,

            showStats: true,
            nodeStats: true,

            showFilters: true,
            nodeFilters: true,

            captionToggle: true,
            edgesToggle: true,
            nodesToggle: true,
            toggleRootNotes: false,

            zoomControls: true         
        };