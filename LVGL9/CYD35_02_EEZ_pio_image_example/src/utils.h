
#include "ui/images.h"
#include "ui/screens.h"
#include "ui/fonts.h"

//------------------------------------------------------------
// List of team names loaded from SD card file PL_teams.txt
// Fixed 20 teams in Premier League +  Select A Team = 21
//------------------------------------------------------------
String arr[21];

//----------------------------------------------------------------
// Get teams data from CSV and store in struct array
// https://github.com/michalmonday/CSV-Parser-for-Arduino/tree/master
//
// Read the data from PL_teams_data_wh_png.csv and store it in list_of_teams
// Headers in CSV files:
// id,team,est,stadium,cupsWon,runnersUp,seasonsInPL,imagePath
//----------------------------------------------------------------
struct PLTeam
{
  String id;
  String team;
  String est;
  String stadium;
  String cupsWon;
  String runnersUp;
  String seasonsInPL;
  String imagePath;

  String describeTeam()
  {
    return id + "," + team + "," + est + "," + stadium + "," + cupsWon + "," + runnersUp + "," + seasonsInPL + "," + imagePath;
  }
} list_of_teams[21];

void populateTeamsStructFromSDCard(const char *path)
{

  // parseRow calls feedRowParser() continuously until it reads a
  // full row or until the rowParserFinished() returns true

  //----------------------------------------------------------------
  // LAZY Just use string for everyting:
  //----------------------------------------------------------------
  CSV_Parser cp(/*format*/ "ssssssss", /*has_header*/ true, /*delimiter*/ ',');

  // The line below (readSDfile) wouldn't work if SD.begin wasn't called before.
  // readSDfile can be used as conditional, it returns 'false' if the file does not exist.
  if (cp.readSDfile(path))
  {
    char **ids = (char **)cp["id"];
    char **teams = (char **)cp["team"];
    char **ests = (char **)cp["est"];
    char **stadiums = (char **)cp["stadium"];
    char **cupsWons = (char **)cp["cupsWon"];
    char **runnersUps = (char **)cp["runnersUp"];
    char **seasonsInPLs = (char **)cp["seasonsInPL"];
    char **imagePaths = (char **)cp["imagePath"];

    // Serial.println("id,team,est,stadium,cupsWon,runnersUp,seasonsInPL,imagePath");
    // Serial.println("1,Arsenal,1886,Emirates Stadium,3,8,33,Arsenal.png");
    // Serial.println("2,Aston Villa,1874,Villa Park,0,0,30,AstonVilla.png");
    if (ids && teams)
    {
      // cp.getRowsCount()
      for (int row = 0; row < cp.getRowsCount(); row++)
      {

        list_of_teams[row].id = ids[row];
        list_of_teams[row].team = teams[row];
        list_of_teams[row].est = ests[row];
        list_of_teams[row].stadium = stadiums[row];
        list_of_teams[row].cupsWon = cupsWons[row];
        list_of_teams[row].runnersUp = runnersUps[row];
        list_of_teams[row].seasonsInPL = seasonsInPLs[row];
        list_of_teams[row].imagePath = imagePaths[row];

        // String line = String(ids[row]);
        // line = line + "," + (teams[row]);
        // line = line + "," + (ests[row]);
        // line = line + "," + (stadiums[row]);
        // line = line + "," + (cupsWons[row]);
        // line = line + "," + (runnersUps[row]);
        // line = line + "," + (seasonsInPLs[row]);
        // line = line + "," + (imagePaths[row]);

        // Serial.print(row);
        // Serial.print(" ");
        // Serial.println(line);

        // Add data to the map
        // d("" + (row + 1), line);

        // Serial.println();
      }
    }
    else
    {
      Serial.println("ERROR: At least 1 of the columns was not found, something went wrong.");
    }

    // output parsed values (allows to check that the file was parsed correctly)
    // cp.print();  // assumes that "Serial.begin()" was called before (otherwise it won't work)
  }
  else
  {
    Serial.println("ERROR: File called '/PL_teams_data_wh.csv' does not exist...");
  }
}

//------------------------------------------------------------
// Tried to load images directly from SD Card
// But I wasn't able to.
// Not sure if CYD supports this or it could be my one isn't
// setup correctly
//------------------------------------------------------------

/*
  This checks if we are able to read the image file
*/
static void checkIfSDCardIsReady(char val, String path)
{
  Serial.println("Path: " + path);
  char buf;
  Serial.println(lv_fs_is_ready(val));
  lv_fs_get_letters(&buf);
  Serial.println(buf);
  lv_fs_file_t f;
  lv_fs_res_t res;
  // res = lv_fs_open(&f, val + ":wink.png", LV_FS_MODE_RD);
  Serial.println("Opening file: " + path);
  res = lv_fs_open(&f, path.c_str(), LV_FS_MODE_RD);
  Serial.println(res);

  // lv_fs_close(&f);
  Serial.println("Opening probably done: " + path);
  Serial.println();
}

char fsIdentifier = 'B';
String drive = String(fsIdentifier) + ":";
static void load_image_sdcard(String path)
{

  // NOTE: Loading direct from SD worked :)
  Serial.println("Loading Images: " + path);
  String fileType = "bmp";
  String folderName = "/bmpTo60/"; // bmpTo60 bmpToJpg seems to work
  path.replace("png", fileType);    // Default used in the CSV file is png
  String fullPath = drive + folderName + path;

  // Check if image exists (If image isn't loading)
  // Serial.println("Check if we can load image from SD");
  // checkIfSDCardIsReady(fsIdentifier, fullPath);
  // Serial.println("Chceck Finished!");

  Serial.println("Setting image src to : " + fullPath);

  // Delete and recreate with new image (Not needed as I'm not able to load image from SD Card)
  // lv_obj_delete(objects.img_logo);
  // lv_image_cache_drop(NULL);
  // lv_obj_t *obj = lv_image_create(lv_screen_active());
  // objects.img_logo = obj;
  // // lv_obj_set_pos(obj, 245, 165);
  // // lv_obj_set_size(obj, 60, 60);
  // lv_obj_set_pos(obj, 191, 87);
  // lv_obj_set_size(obj, 115, 125);
  // // lv_img_set_zoom(obj, 356);
  // lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_image_cache_drop(NULL);
  lv_image_set_src(objects.img_logo, fullPath.c_str());

  Serial.println("Load image from SD Done");
}

void populateListOfTeamsArrayFromSDCard(fs::FS &fs, const char *path)
{
  Serial.printf("\nReading getListOfPremierLeagueTeams file: %s\n", path);

  File file = fs.open(path);
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }
  Serial.println("Read populateListOfTeamsArrayFromSDCard from file and store in array: ");

  // Read file and populate an array
  String val1 = "Select A Team";
  val1.trim();
  arr[0] = val1;

  // Get the team names and store in arr
  int indexPosition = 1;
  while (file.available())
  {
    String buffer = file.readStringUntil('\n');
    arr[indexPosition] = buffer;
    indexPosition = indexPosition + 1;
  }
  file.close();
}

void add_symbols()
{
  // Set symbol
  //  lv_image_set_src(objects.img_symbol_2, LV_SYMBOL_OK);
  lv_image_set_src(objects.img_symbol_1, LV_SYMBOL_OK);
  lv_image_set_src(objects.img_symbol_3, LV_SYMBOL_PLUS);
  lv_image_set_src(objects.img_symbol_4, LV_SYMBOL_MINUS);
  lv_image_set_src(objects.img_symbol_5, LV_SYMBOL_UPLOAD);
}

//------------------------------------------------------------
// Populates the teams dropdown with list of names from SD Card
//------------------------------------------------------------
void populate_teams_dd()
{

  // Populate the TEAM dropdown list
  Serial.println("\n\nTeams data from SD card");
  int pos = 1;
  for (String option : arr)
  {
    Serial.println(option);
    option.trim();
    lv_dropdown_add_option(objects.dd_team_names, option.c_str(), pos);
    pos = pos + 1;
  }
}

//------------------------------------------------------------
// Populates the teams ROLLER with list of names from SD Card
//------------------------------------------------------------
void populate_teams_roller()
{

  // Populate the TEAM dropdown list
  Serial.println("\n\nTeams data from SD card");
  String options;
  int count = 0;
  for (String option : arr)
  {
    option.trim();

    if (!option.isEmpty())
    {
      if (count == 0)
      {
        options = "Select A Team";
      }
      else
      {
        options = options + "\n" + option;
      }
      count = count + 1;
      Serial.println(option);
    }
  }
  Serial.println(options);
  lv_roller_set_options(objects.roler_team_names, options.c_str(), LV_ROLLER_MODE_INFINITE);
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.name(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());

      // Serial.print(file);
    }
    file = root.openNextFile();
  }
}

//------------------------------------------------------------
// EVENT HANDLERS
//------------------------------------------------------------

PLTeam selectedTeam;

// static void update_large_image_event_handler(lv_event_t *e)
// {
//   lv_event_code_t code = lv_event_get_code(e);             // Get the event code
//   lv_obj_t *dd = (lv_obj_t *)lv_event_get_target(e);       // Switch that generated the event
//   lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e); // Label or other UI elements we want to update (Optional)

//   Serial.println("Manually Fired Event ");
//   if (code == LV_EVENT_VALUE_CHANGED)
//   {
//     // char buf[24];
//     // lv_dropdown_get_selected_str(dd, buf, sizeof(buf));
//     // LV_LOG_USER("Option: %s", buf);
//     // Serial.println(buf);

//     if (selectedTeam.imagePath != NULL)
//     {
//       // Show team data
//       // int selectedIndex = lv_dropdown_get_selected(dd);
//       // PLTeam aTeam = list_of_teams[selectedIndex - 1];
//       // Serial.println(aTeam.describeTeam());

//       // Load large image
//       //  load_image_sdcard(selectedTeam.imagePath);
//       String path = selectedTeam.imagePath;
//       Serial.println("Loading Images: " + path);
//       path.replace("png", "bmp");
//       String folderName = "/bmp/"; // bmpTo60 seems to work
//       String fullPathLarge = drive + "/bmp/" + path;
//       lv_image_set_src(objects.img_logo_2, NULL);
//       lv_image_set_src(objects.img_logo_2, fullPathLarge.c_str());
//     }
//     else
//     {
//       Serial.println("DON'T DO ANYTHING");
//     }
//   }
// }
static void clear_all_data()
{
  lv_label_set_text(objects.label_stadium_name, "");
  lv_label_set_text(objects.label_established, "");
  lv_label_set_text(objects.label_titles_won, "");
  lv_label_set_text(objects.label_runners_up, "");
  lv_label_set_text(objects.label_years_in_pl, "");

  //
  lv_image_set_src(objects.img_logo, NULL);
}

static void roller_event_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);             // Get the event code
  lv_obj_t *roller = (lv_obj_t *)lv_event_get_target(e);   // Switch that generated the event
  lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e); // Label or other UI elements we want to update (Optional)

  Serial.println("Dropdown Team Selected");
  if (code == LV_EVENT_VALUE_CHANGED)
  {

    char buf[24];
    lv_roller_get_selected_str(roller, buf, sizeof(buf));
    LV_LOG_USER("Option: %s", buf);
    Serial.println(buf);

    if (String(buf) != "Select A Team")
    {
      // Show team data
      int selectedIndex = lv_roller_get_selected(roller);
      PLTeam aTeam = list_of_teams[selectedIndex - 1];
      Serial.println(aTeam.describeTeam());

      // load_image_sdcard("/Wolves.png");
      load_image_sdcard(aTeam.imagePath);

      // set roller value on dd selected index
      // lv_dropdown_set_selected(objects.dd_team_names, selectedIndex);

      // lv_label_set_text(objects.label_detail_1, aTeam.team.c_str());
      aTeam.stadium.replace("Stadium", "");
      lv_label_set_text(objects.label_stadium_name, aTeam.stadium.c_str());
      lv_label_set_text(objects.label_established, aTeam.est.c_str());
      lv_label_set_text(objects.label_titles_won, aTeam.cupsWon.c_str());
      lv_label_set_text(objects.label_runners_up, aTeam.runnersUp.c_str());
      lv_label_set_text(objects.label_years_in_pl, aTeam.seasonsInPL.c_str());

      // Register event handler
      // lv_image_set_src(objects.img_logo_2, NULL);
      // lv_obj_add_event_cb(objects.img_logo_2, update_large_image_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
      // delay(100);

      // Fire event to update large logo
      selectedTeam = aTeam;
      // lv_obj_send_event(objects.img_logo_2, LV_EVENT_VALUE_CHANGED, NULL);
    }
    else
    {
      clear_all_data();
      Serial.println("DON'T DO ANYTHING");
    }
  }
}

static void dd_event_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);             // Get the event code
  lv_obj_t *dd = (lv_obj_t *)lv_event_get_target(e);       // Switch that generated the event
  lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e); // Label or other UI elements we want to update (Optional)

  Serial.println("Dropdown Team Selected");
  if (code == LV_EVENT_VALUE_CHANGED)
  {

    char buf[24];
    lv_dropdown_get_selected_str(dd, buf, sizeof(buf));
    LV_LOG_USER("Option: %s", buf);
    Serial.println(buf);

    if (String(buf) != "Select A Team" && String(buf) != "")
    {
      // Show team data
      int selectedIndex = lv_dropdown_get_selected(dd);
      PLTeam aTeam = list_of_teams[selectedIndex - 1];
      Serial.println(aTeam.describeTeam());

      // set roller selected index
      lv_roller_set_selected(objects.roler_team_names, selectedIndex, LV_ANIM_OFF);

      // load_image_sdcard("/Wolves.png");
      load_image_sdcard(aTeam.imagePath);

      // lv_label_set_text(objects.label_detail_1, aTeam.team.c_str());
      aTeam.stadium.replace("Stadium", "");
      lv_label_set_text(objects.label_stadium_name, aTeam.stadium.c_str());
      lv_label_set_text(objects.label_established, aTeam.est.c_str());
      lv_label_set_text(objects.label_titles_won, aTeam.cupsWon.c_str());
      lv_label_set_text(objects.label_runners_up, aTeam.runnersUp.c_str());
      lv_label_set_text(objects.label_years_in_pl, aTeam.seasonsInPL.c_str());

      // Register event handler
      // lv_image_set_src(objects.img_logo_2, NULL);
      // lv_obj_add_event_cb(objects.img_logo_2, update_large_image_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
      // delay(100);

      // Fire event to update large logo
      selectedTeam = aTeam;
      // lv_obj_send_event(objects.img_logo_2, LV_EVENT_VALUE_CHANGED, NULL);
    }
    else
    {
      Serial.println("DON'T DO ANYTHING");
    }
  }
}
