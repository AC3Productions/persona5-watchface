module.exports = 
[
    {
        "type": "heading",
        "defaultValue": "Watchface Settings"
    },
    {
        "type": "text",
        "defaultValue": "Customize your watchface appearance and preferences."
    },
    {
        "type": "section",
        "items": 
        [
            {
                "type": "select",
                "messageKey": "BackgroundMode",
                "defaultValue": "sunrise_sunset",
                "label": "Background Mode",
                "options": 
                [
                    {
                        "label": "Day Only",
                        "value": "day_only"
                    },
                    {
                        "label": "Night Only",
                        "value": "night_only"
                    },
                    {
                        "label": "Constant Times",
                        "value": "constant_times"
                    },
                    {
                        "label": "Sunrise/Sunset",
                        "value": "sunrise_sunset"
                    }
                ]
            },
            {
                "type": "slider",
                "messageKey": "ConstantSunrise",
                "defaultValue": "5",
                "label": "Sunrise Time",
                "description": "Only applies to \"Constant Times\" option above.",
                "min": 0,
                "max": 23
            },
            {
                "type": "slider",
                "messageKey": "ConstantSunset",
                "defaultValue": "19",
                "label": "Sunset Time",
                "description": "Only applies to \"Constant Times\" option above.",
                "min": 0,
                "max": 23
            },
            {
                "type": "toggle",
                "messageKey": "TemperatureUnit",
                "label": "Use Fahrenheit",
                "defaultValue": true
            },
            {
                "type": "toggle",
                "messageKey": "ShowTimeOfDay",
                "label": "Show Time of Day",
                "defaultValue": true,
                "description": "Whether to show the \"Afternoon\"/\"Evening\"-style Time of Day display."
            },
            {
                "type": "toggle",
                "messageKey": "DisableAnimation",
                "label": "Disable Animation",
                "defaultValue": false,
                "description": "Disable the animation on the weather conditions display. Improves battery performance."
            }
        ]
    },
    {
        "type": "submit",
        "defaultValue": "Save Settings"
    }
];