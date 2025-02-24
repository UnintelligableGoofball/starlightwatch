module.exports = [
    {
        "type": "heading",
        "defaultValue": "App Configuration"
    },
    {
        "type": "text",
        "defaultValue": "Hatsune Miku watchface settings."
    },
    /*{
        "type": "section",
        "items": [
          {
            "type": "heading",
            "defaultValue": "Theme"
          },
          {
            "type": "toggle",
            "messageKey": "Theme",
            "label": "Invert Theme",
            "defaultValue": false
          }
        ]
    },*/
    {
        "type": "section",
        "items": [
          {
            "type": "heading",
            "defaultValue": "Animation"
          },
          {
            "type": "select",
            "messageKey": "AnimationSpeed",
            "defaultValue": "850",
            "label": "Animation Speed",
            "options": [
              { 
                "label": "Off", 
                "value": "0" 
              },
              { 
                "label": "Slow",
                "value": "850" 
              },
              { 
                "label": "Medium",
                "value": "600" 
              },
              { 
                "label": "Fast",
                "value": "300" 
              }
            ]
          },
          {
            "type": "slider",
            "messageKey": "AnimationDelay",
            "defaultValue": 250,
            "label": "Slider",
            "description": "Animation delay",
            "min": 0,
            "max": 1000,
            "step": 10
          }
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save"
    }
    ]