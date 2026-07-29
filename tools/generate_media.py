from pathlib import Path

file = open("media.json", "w")
file.write("""{
        "media": [
        """)

# Font
file.write("""{
            "characterRegex": "[:0-9]",
            "type": "font",
            "name": "FONT_EARWIG_FACTORY_48",
            "file": "fonts/earwig-factory-rg.ttf"
        },
        {
            "characterRegex": "[:0-9]",
            "type": "font",
            "name": "FONT_POLAND_CANNED_INTO_KAITO_40",
            "file": "fonts/PolandCannedIntoKaito.ttf"
        },
        {
            "characterRegex": "[0-9°CF]",
            "type": "font",
            "name": "FONT_PUBLIC_SANS_BLACK_30",
            "file": "fonts/PublicSans-BlackItalic.ttf"
        },
        {
            "type": "png",
            "name": "DAY_BG",
            "file": "images/day_bg-small.png"
        },
        {
            "type": "png",
            "name": "NIGHT_BG",
            "file": "images/night_bg-small.png"
        },
        {
            "type": "png",
            "name": "ICON",
            "file": "images/icon.png",
            "menuIcon": true
        }""")

# days
for i in range(1,32):
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"DAY_{i}_TOP"' + """,
            "file": """ + f'"images/day/{i}Top.png"' + """
        }""")
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"DAY_{i}_MID"' + """,
            "file": """ + f'"images/day/{i}.png"' + """
        }""")
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"DAY_{i}_BOTTOM"' + """,
            "file": """ + f'"images/day/{i}Bottom.png"' + """
        }""")

# months
for i in range(1,13):
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"MONTH_{i}_TOP"' + """,
            "file": """ + f'"images/month/{i}Top.png"' + """
        }""")
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"MONTH_{i}_MID"' + """,
            "file": """ + f'"images/month/{i}.png"' + """
        }""")
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"MONTH_{i}_BOTTOM"' + """,
            "file": """ + f'"images/month/{i}Bottom.png"' + """
        }""")
    
# 2-digit months
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"MONTH_{i}_TOP_2DIGIT"' + """,
            "file": """ + f'"images/month/{i}Top_2digit.png"' + """
        }""")
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"MONTH_{i}_MID_2DIGIT"' + """,
            "file": """ + f'"images/month/{i}_2digit.png"' + """
        }""")
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"MONTH_{i}_BOTTOM_2DIGIT"' + """,
            "file": """ + f'"images/month/{i}Bottom_2digit.png"' + """
        }""")


# weekdays
weekdays = ['Sunday', 'Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday']

for i in weekdays:
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"WEEK_{i.upper()}_TOP"' + """,
            "file": """ + f'"images/week/{i}Top.png"' + """
        }""")
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"WEEK_{i.upper()}_MID"' + """,
            "file": """ + f'"images/week/{i}.png"' + """
        }""")
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"WEEK_{i.upper()}_BOTTOM"' + """,
            "file": """ + f'"images/week/{i}Bottom.png"' + """
        }""")


# weather (apng)
weather = ['sun', 'cloud', 'rain', 'snow', 'unknown']
for i in weather:
    file.write(""",
        {
            "type": "raw",
            "name": """ + f'"WEATHER_{i.upper()}"' + """,
            "file": """ + f'"images/weather/{i}.apng"' + """
        }""")
    file.write(""",
        {
            "type": "raw",
            "name": """ + f'"WEATHER_{i.upper()}_2DIGIT"' + """,
            "file": """ + f'"images/weather/{i}_2digit.apng"' + """
        }""")

# time of day
time_of_day = ['early_morning', 'morning', 'daytime', 'afternoon', 'evening']
for i in time_of_day:
    file.write(""",
        {
            "type": "png",
            "name": """ + f'"TIME_{i.upper()}"' + """,
            "file": """ + f'"images/time/{i}.png"' + """
        }""")

# close out the array
file.write("""
    ]
}""")
file.close()
print("Media json created successfully!")


## Now the resource arrays
file = open("resources.generated.h", "w")

file.write("""static const uint32_t DayResourceIDs[3][31] = {
    {\n""")
for i in range(1,32):
    file.write(f'\t\tRESOURCE_ID_DAY_{i}_BOTTOM,\n')
file.write("\t},\n\t{\n")
for i in range(1,32):
    file.write(f'\t\tRESOURCE_ID_DAY_{i}_MID,\n')
file.write("\t},\n\t{\n")
for i in range(1,32):
    file.write(f'\t\tRESOURCE_ID_DAY_{i}_TOP,\n')
file.write("\t}\n};\n\n")

file.write("""static const uint32_t MonthResourceIDs[3][12] = {
    {\n""")
for i in range(1,13):
    file.write(f'\t\tRESOURCE_ID_MONTH_{i}_BOTTOM,\n')
file.write("\t},\n\t{\n")
for i in range(1,13):
    file.write(f'\t\tRESOURCE_ID_MONTH_{i}_MID,\n')
file.write("\t},\n\t{\n")
for i in range(1,13):
    file.write(f'\t\tRESOURCE_ID_MONTH_{i}_TOP,\n')
file.write("\t}\n};\n\n")

file.write("""static const uint32_t Month2DigitResourceIDs[3][12] = {
    {\n""")
for i in range(1,13):
    file.write(f'\t\tRESOURCE_ID_MONTH_{i}_BOTTOM_2DIGIT,\n')
file.write("\t},\n\t{\n")
for i in range(1,13):
    file.write(f'\t\tRESOURCE_ID_MONTH_{i}_MID_2DIGIT,\n')
file.write("\t},\n\t{\n")
for i in range(1,13):
    file.write(f'\t\tRESOURCE_ID_MONTH_{i}_TOP_2DIGIT,\n')
file.write("\t}\n};\n\n")

file.write("""static const uint32_t WeekdayResourceIDs[3][7] = {
    {\n""")
for i in weekdays:
    file.write(f'\t\tRESOURCE_ID_WEEK_{i.upper()}_BOTTOM,\n')
file.write("\t},\n\t{\n")
for i in weekdays:
    file.write(f'\t\tRESOURCE_ID_WEEK_{i.upper()}_MID,\n')
file.write("\t},\n\t{\n")
for i in weekdays:
    file.write(f'\t\tRESOURCE_ID_WEEK_{i.upper()}_TOP,\n')
file.write("\t}\n};\n\n")

file.write('static const uint32_t WeatherResourceIDs[] = {\n')
for i in weather:
    file.write(f'\tRESOURCE_ID_WEATHER_{i.upper()},\n')
file.write("};\n\n")

file.write('static const uint32_t Weather2DigitResourceIDs[] = {\n')
for i in weather:
    file.write(f'\tRESOURCE_ID_WEATHER_{i.upper()}_2DIGIT,\n')
file.write("};\n\n")

file.write('static const uint32_t TimeOfDayResourceIDs[] = {\n')
for i in time_of_day:
    file.write(f'\tRESOURCE_ID_TIME_{i.upper()},\n')
file.write("};\n\n")

file.close()

print ("Resources header created successfully!")