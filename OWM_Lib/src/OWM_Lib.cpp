#include <Arduino.h>
#include <string.h>
#include "OWM_Lib.h"
#include "ArduinoJson.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#elif defined ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#endif
#include <WiFiClient.h>
// реализация методов
OWM_Lib::OWM_Lib(String lat, String lon, String appid) // конструктор
{
    _lat = lat;
    _lon = lon;
    _appid = appid;
}
// =======================================================================
// Расшифровка погоды текущей и суточной в строку _Scroll_tape с сайта openweathermap.org
// =======================================================================
void OWM_Lib::convWeatherRealDay()
{
    _Scroll_tape = " Тн:" + _CheckFloatNum(Real.temp) + char(128) + " (" + _CheckFloatNum(Real.feels_like) + char(128) + ") [" + _CheckFloatNum(Real.temp_min) + char(128) + " ~ " + _CheckFloatNum(Real.temp_max) + char(128) + "] ";
    _Scroll_tape += Real.weather_description;
    _Scroll_tape += " RH:" + String(Real.humidity) + "%";
    _Scroll_tape += " P:" + String(Real.pressure) + "mmHg"; //String(round(pressure),0) + "mmHg";
    _Scroll_tape += " Cl:" + String(Real.clouds_all) + "% W:";
    Real.str_Wind = _CheckFloatNum(Real.wind_speed) + "м/с ";
    if (Real.wind_deg > 0 && Real.wind_deg < 90)
        Real.str_Wind += "СВ";
    if (Real.wind_deg > 270 && Real.wind_deg < 360)
        Real.str_Wind += "СЗ";
    if (Real.wind_deg > 90 && Real.wind_deg < 180)
        Real.str_Wind += "ЮВ";
    if (Real.wind_deg > 180 && Real.wind_deg < 270)
        Real.str_Wind += "ЮЗ";
    if (Real.wind_deg == 0)
        Real.str_Wind += "С";
    if (Real.wind_deg == 90)
        Real.str_Wind += "В";
    if (Real.wind_deg == 180)
        Real.str_Wind += "Ю";
    if (Real.wind_deg == 270)
        Real.str_Wind += "З";
    _Scroll_tape += Real.str_Wind;
    String str_Rain = "";
    if (Real.rain_1h > 0)
    {
        _Scroll_tape += " Дождь:" + _CheckFloatNum(Real.rain_1h) + "мм";
        str_Rain = "Дождь:" + _CheckFloatNum(Real.rain_1h) + "мм";
    }
    if (Day.snow_3h > 0)
    {
        if (Real.temp_max > -5)
        {
            _Scroll_tape += " *" + _CheckFloatNum(Real.snow_1h * 1.5) + "cм"; // 1.5 см снега
            str_Rain += " *" + _CheckFloatNum(Real.snow_1h * 1.5) + "cм";     // 1.5 см снега
        }
        else
        {
            _Scroll_tape += " *" + _CheckFloatNum(Real.snow_1h * 2.5) + "cм"; //2.5 см снега
            str_Rain += " *" + _CheckFloatNum(Real.snow_1h * 2.5) + "cм";     //2.5 см снега
        }
    }
    _Scroll_tape += " Прогноз ";
    _Scroll_tape += " T:" + _CheckFloatNum(Day.main_temp_min) + " ~ " + _CheckFloatNum(Day.main_temp_max) + char(128);
    _Scroll_tape += " (" + _CheckFloatNum(Day.main_feels_like) + char(128) + ") ";
    _Scroll_tape += Day.weather_description + " W:";
    Day.str_Wind = "";
    if (Day.wind_deg > 0 && Day.wind_deg < 90)
        Day.str_Wind += "СВ";
    if (Day.wind_deg > 270 && Day.wind_deg < 360)
        Day.str_Wind += "СЗ";
    if (Day.wind_deg > 90 && Day.wind_deg < 180)
        Day.str_Wind += "ЮВ";
    if (Day.wind_deg > 180 && Day.wind_deg < 270)
        Day.str_Wind += "ЮЗ";
    if (Day.wind_deg == 0)
        Day.str_Wind += "С";
    if (Day.wind_deg == 90)
        Day.str_Wind += "В";
    if (Day.wind_deg == 180)
        Day.str_Wind += "Ю";
    if (Day.wind_deg == 270)
        Day.str_Wind += "З";
    _Scroll_tape += _CheckFloatNum(Day.wind_speed) + "м/с " + Day.str_Wind;
    Day.str_Wind = _CheckFloatNum(Day.wind_speed) + " " + Day.str_Wind;
    Day.str_Rain = "";
    if (Day.rain_3h > 0)
    {
        _Scroll_tape += " Дождь:" + _CheckFloatNum(Day.rain_3h) + "мм";
        Day.str_Rain = "Дождь:" + _CheckFloatNum(Day.rain_3h) + "мм";
    }
    if (Day.snow_3h > 0)
    {
        if (Day.main_temp_max > -5)
        {
            _Scroll_tape += " *" + _CheckFloatNum(Day.snow_3h * 1.5) + "cм"; // 1.5 см снега
            Day.str_Rain += " *" + _CheckFloatNum(Day.snow_3h * 1.5) + "cм"; // 1.5 см снега
        }
        else
        {
            _Scroll_tape += " *" + _CheckFloatNum(Day.snow_3h * 2.5) + "cм"; //2.5 см снега
            Day.str_Rain += " *" + _CheckFloatNum(Day.snow_3h * 2.5) + "cм"; //2.5 см снега
        }
    }
    _Tbot_Weather_string = " Тн: " + _CheckFloatNum(Real.temp) + "гр.C";
    _Tbot_Weather_string += "\nТощ:" + _CheckFloatNum(Real.feels_like) + "гр.C";
    _Tbot_Weather_string += "\nВлажность: " + String(Real.humidity) + " %";
    _Tbot_Weather_string += "\nДавление:" + String(Real.pressure) + "mmHg";
    _Tbot_Weather_string += "\nОблачность:" + String(Real.clouds_all) + " %";
    _Tbot_Weather_string += "\n" + Real.weather_description;
    _Tbot_Weather_string += "\nВетер:" + Real.str_Wind;
    _Tbot_Weather_string += "\n" + str_Rain;
    _Tbot_Weather_string += "\n\nПрогноз ";
    _Tbot_Weather_string += "\nTн:" + _CheckFloatNum(Day.main_temp_min) + " ~ " + _CheckFloatNum(Day.main_temp_max) + "гр.C";
    _Tbot_Weather_string += "\nТощ:" + _CheckFloatNum(Day.main_feels_like) + "гр.C";
    _Tbot_Weather_string += "\nВлажность: " + String(Day.main_humidity) + " %";
    _Tbot_Weather_string += "\nДавление:" + String(Day.main_pressure) + "mmHg";
    _Tbot_Weather_string += "\nОблачность:" + String(Day.clouds_all) + " %";
    _Tbot_Weather_string += "\n" + Day.weather_description;
    _Tbot_Weather_string += "\nВетер:" + Day.str_Wind;
    _Tbot_Weather_string += "\n" + Day.str_Rain;

    //Draw_on_Display.Text.Scroll_Tape = utf8rus(Loc_Time.Data_string + " " + _Scroll_tape);
}
// =======================================================================
//строка погоды для телеграмм
// =======================================================================
String OWM_Lib::Telegram_string()
{
    return _Tbot_Weather_string;
}
// =======================================================================
//строка погоды для бегущей строки
// =======================================================================
String OWM_Lib::Scroll_tape()
{

    return _Scroll_tape;
}
// =======================================================================
// каждые 15 мин получаем данные текущей погоды и каждый час прогноз погоды
// =======================================================================
uint8_t OWM_Lib::get_Weather()
{
    if (Real.Synchr_www < millis())
    {
        if (!_get_Weather(false))
        { // при ошибке получения текущей погоды ...
            Err_WiFi_Connect++;
            Real.Synchr_www = millis() + 120000; //2мин;
            return Errors::Get_Real;
        }
        // заполняем буфера температуры и давления
        Real.Synchr_www = millis() + 900000; //15min

        Graph_Data[97].Temperature_GR = int(Real.temp * 10);            //+random(3)*2;
        Graph_Data[97].Temperature_Feel_GR = int(Real.feels_like * 10); //+random(3)*2;
        Graph_Data[97].pressure_GR = Real.pressure - 700;
        Graph_Data[97].humidity_GR = Real.humidity;
        Graph_Data[97].Graphic_Time = NTP_Local;

        for (uint8_t Loop_i = 0; Loop_i <= 96; Loop_i++)
        {

            Graph_Data[Loop_i].Temperature_GR = Graph_Data[Loop_i + 1].Temperature_GR;
            Graph_Data[Loop_i].Temperature_Feel_GR = Graph_Data[Loop_i + 1].Temperature_Feel_GR;
            Graph_Data[Loop_i].pressure_GR = Graph_Data[Loop_i + 1].pressure_GR;
            Graph_Data[Loop_i].humidity_GR = Graph_Data[Loop_i + 1].humidity_GR;
            Graph_Data[Loop_i].Graphic_Time = Graph_Data[Loop_i + 1].Graphic_Time;
        }
        Err_WiFi_Connect = 0;
        return Errors::Real_OK;
    }
    // =======================================================================
    // каждые час получаем данные прогноз погоды
    // =======================================================================
    if (Day.Synchr_www < millis())
    {
        if (!_get_Weather(true))
        {
            Day.Synchr_www = millis() + 120000; //2min
            return Errors::Get_Day;
        }

        Day.Synchr_www = millis() + 3600000; //1hour
        convWeatherRealDay();
        return Errors::Day_OK;
    }
    return Errors::No_synhr;
}

// =======================================================================
// чтение с сервера погоды
// =======================================================================
bool OWM_Lib::_get_Weather(bool Real_Day)
{
    String line = "";
    if (!Real_Day)
    {
        Real.fl_Get = false; // Сбрасываем флаг принятия погоды
    }
    else
    {
        Day.fl_Get = false; // Сбрасываем флаг принятия погоды
    }
    line = ""; // переменная для чтения погоды из инета
    if ((WiFi.status() != WL_CONNECTED))
        return false; //Check the current connection status
    HTTPClient http;
    if (!Real_Day)
    {
        http.begin("http://api.openweathermap.org/data/2.5/weather?lat=" + _lat + "&lon=" + _lon + "&appid=" + _appid + "&lang=ru&units=metric");
    }
    else
    {
        http.begin("http://api.openweathermap.org/data/2.5/forecast?lat=" + _lat + "&lon=" + _lon + "&appid=" + _appid + "&lang=ru&units=metric&cnt=1");
    }
    int httpCode = http.GET(); //Make the request
    if (httpCode == 200)
    { //Check for the returning code
        line = http.getString();
        http.end(); //Free the resources
    }
    else
    {
        (!Real_Day) ? Serial.println("Weather Real read error") : Serial.println("Weather Day read error");
        http.end(); //Free the resources
        return false;
    }
    line.replace("\n", "");
    line.replace("\r", "");
    line.replace("\"", "'");
    Serial.println(line);
    //Serial.println(String(Real_Day)+" Weather received in full ____________________________________________________________________________"+String(jioh));

    //JSON 5 ver
    /* 	StaticJsonBuffer<2000> jsonBuffer;                          /// буфер на 2000 символов
  	JsonObject& root = jsonBuffer.parseObject(line);     // скармливаем String
  	if (!root.success()) return;
  */
    //JSON 6 ver

    //StaticJsonDocument<1500> root;
    DynamicJsonDocument root(1500);
    DeserializationError error = deserializeJson(root, line);
    if (error)
    {
        switch (error.code())
        {
        case DeserializationError::Ok:
            //Serial.println("OWM Real error: error JSON (Deserialization succeeded)");
            break;
        case DeserializationError::InvalidInput:
            Serial.println("   error JSON (Invalid input!)");
            return false;
        case DeserializationError::NoMemory:
            Serial.println("   error JSON (Not enough memory)");
            return false;
        default:
            Serial.println("   error JSON (Deserialization failed)");
            return false;
        }
        return false;
    }
    if (!Real_Day)
    {
        Real.coord_lon = root["coord"]["lon"];                                     //37.66 Географическое положение города, долгота
        Real.coord_lat = root["coord"]["lat"];                                     //55.87 Географическое положение города, широта
        Real.weather_id = root["weather"][0]["id"];                                //520 Идентификатор погодных условий
        Real.weather_main = root["weather"][0]["main"].as<String>();               //Rain Группа погодных параметров (Дождь, Снег, Экстрим и др.)
        Real.weather_description = root["weather"][0]["description"].as<String>(); //небольшой проливной дождь Погодные условия в группе. Вы можете получить вывод на своем языке. Выучить больше
        Real.weather_icon = root["weather"][0]["icon"].as<String>();               //"09d" Идентификатор значка погоды
        Real.temp = root["main"]["temp"];                                          //11.93 Температура. Единица измерения по умолчанию: Кельвин, Метрическая система: Цельсий, Имперская система: Фаренгейт.
        Real.feels_like = root["main"]["feels_like"];                              //8.43 Температура. Этот температурный параметр объясняет человеческое восприятие погоды. Единица измерения по умолчанию: Кельвин, Метрическая система: Цельсий, Имперская система: Фаренгейт.
        Real.temp_min = root["main"]["temp_min"];                                  //11 Минимальная температура на данный момент. Это минимальная наблюдаемая в настоящее время температура (в пределах крупных мегаполисов и городских территорий). Единица измерения по умолчанию: Кельвин, Метрическая система: Цельсий, Имперская система: Фаренгейт.
        Real.temp_max = root["main"]["temp_max"];                                  //12.78 Максимальная температура на данный момент. Это максимальная наблюдаемая в настоящее время температура (в пределах крупных мегаполисов и городских территорий). Единица измерения по умолчанию: Кельвин, Метрическая система: Цельсий, Имперская система: Фаренгейт.
        Real.pressure = root["main"]["pressure"];                                  //1013 Атмосферное давление (на уровне моря, если нет данных sea_level или grnd_level), гПа
        Real.pressure = (uint16_t)(Real.pressure / 1.3332239 - 12);
        Real.humidity = root["main"]["humidity"]; //87 Влажность,%
        Real.visibility = root["visibility"];     // видимость
        //uint16_t Real.sea_level;// Атмосферное давление на уровне моря, гПа
        //uint16_t Real.grnd_level;// Атмосферное давление на уровне земли, гПа
        Real.wind_speed = root["wind"]["speed"]; //5 Скорость ветра. Единица измерения по умолчанию: метр / сек, метрическая система: метр / сек, британская система мер: мили / час.
        Real.wind_deg = root["wind"]["deg"];     //270 Направление ветра, градусы (метеорологические)
        Real.clouds_all = root["clouds"]["all"]; //75 Облачность,%
        //uint8_t Real.rain_1h;                    //x10 Объем дождя за последний час, мм
        //uint8_t Real.rain_3h;//x10 Объем дождя за последние 3 часа, мм
        //uint8_t Real.snow_1h; //x10 Объем снега за 1 час, мм
        //uint8_t Real.snow_3h;//x10 Объем снега за последние 3 часа, мм
        Real.dt = root["dt"];                  //1599569691 Время расчета данных, unix, UTC
        Real.sunrise = root["sys"]["sunrise"]; //1599533286 Время восхода, unix, UTC
        Real.sunrise += 10800;
        Real.sunset = root["sys"]["sunset"]; //1599581172 Время заката, unix, UTC
        Real.sunset += 10800;
        Real.city_id = root["id"];             //527981 ID города
        Real.name = root["name"].as<String>(); //Медведково Название города
    }
    else
    {
        Day.dt = root["list"][0]["dt"];                                     //Время прогнозирования данных, Unix, UTC
        Day.main_temp = root["list"][0]["main"]["temp"];                    //Температура_ Единица измерения по умолчанию: Кельвин, Метрическая система: Цельсий, Имперская система: Фаренгейт_
        Day.main_feels_like = root["list"][0]["main"]["feels_like"];        //Этот температурный параметр объясняет человеческое восприятие погоды_ Единица измерения по умолчанию: Кельвин, Метрическая система: Цельсий, Имперская система: Фаренгейт_
        Day.main_temp_min = root["list"][0]["main"]["temp_min"];            //Минимальная температура на момент расчета_ Это минимальная прогнозируемая температура (в пределах крупных мегаполисов и городских территорий), используйте этот параметр по желанию_ Единица измерения по умолчанию: Кельвин, Метрическая система: Цельсий, Имперская система: Фаренгейт_
        Day.main_temp_max = root["list"][0]["main"]["temp_max"];            //Максимальная температура на момент расчета_ Это максимальная прогнозируемая температура (в пределах крупных мегаполисов и городских территорий), используйте этот параметр по желанию_ Единица измерения по умолчанию: Кельвин, Метрическая система: Цельсий, Имперская система: Фаренгейт_
        Day.main_pressure = root["list"][0]["main"]["pressure"];            //Атмосферное давление на уровне моря по умолчанию, гПа
        Day.main_pressure = (uint16_t)(Day.main_pressure / 1.3332239 - 12); // прогноз Давление
        Day.main_sea_level = root["list"][0]["main"]["sea_level"];          //Атмосферное давление на уровне моря, гПа
        Day.main_sea_level = (uint16_t)(Day.main_sea_level / 1.3332239 - 12);
        Day.main_grnd_level = root["list"][0]["main"]["grnd_level"];                         //Атмосферное давление на уровне земли, гПа
        Day.main_grnd_level = (uint16_t)(Day.main_grnd_level / 1.3332239);                   //- 12);
        Day.main_humidity = root["list"][0]["main"]["humidity"];                             //Влажность,%
        Day.main_temp_kf = root["list"][0]["main"]["temp_kf"];                               //Внутренний параметр
        Day.weather_id = root["list"][0]["weather"][0]["id"];                                //Идентификатор погодных условий
        Day.weather_main = root["list"][0]["weather"][0]["main"].as<String>();               //Группа погодных параметров (Дождь, Снег, Экстрим и др_)
        Day.weather_description = root["list"][0]["weather"][0]["description"].as<String>(); //Погодные условия в группе_ Вы можете получить вывод на своем языке_ Узнать больше _
        Day.weather_icon = root["list"][0]["weather"][0]["icon"].as<String>();               //Идентификатор значка погоды
        Day.clouds_all = root["list"][0]["clouds"]["all"];                                   //Облачность,%
        Day.wind_speed = root["list"][0]["wind"]["speed"];                                   //Скорость ветра_ Единица измерения по умолчанию: метр / сек, метрическая система: метр / сек, британская система мер: мили / час_
        Day.wind_deg = root["list"][0]["wind"]["deg"];                                       //Направление ветра, градусы (метеорологические)
        Day.rain_3h = root["list"][0]["rain"]["3h"];                                         //Объем дождя за последний час, мм
        Day.snow_3h = root["list"][0]["snow"]["3h"];                                         //Объем снега за последний час
        //Day.visibility = root["list"][0]["visibility"];//Средняя видимость, метры
        Day.pop = root["list"][0]["pop"];                         //Вероятность выпадения осадков
        Day.sys_pod = root["list"][0]["sys"]["pod"].as<String>(); //Часть дня (n - ночь, d - день)
        Day.dt_txt = root["list"][0]["dt_txt"].as<String>();      //Время прогнозирования данных, ISO, UTC
        Day.city_id = root["city"]["id"];                         //Идентификатор города (доступен, если вызов API был выполнен с помощью идентификатора города )
        Day.city_name = root["city"]["name"].as<String>();        //Название города
        Day.city_coord_lat = root["city"]["coord"]["lat"];        //Географическое положение города, широта
        Day.city_coord_lon = root["city"]["coord"]["lon"];        //Географическое положение города, долгота
        Day.country = root["city"]["country"].as<String>();       //Код страны (Великобритания, Япония и т_ Д_)
        Day.timezone = root["city"]["timezone"];                  //Сдвиг в секундах от UTC
        Day.sunrise = root["city"]["sunrise"];                    //Время восхода, Unix, UTC
        Day.sunrise += Day.timezone;
        Day.sunset = root["city"]["sunset"]; //Время заката, Unix, UTC
        Day.sunset += Day.timezone;
        Day.fl_Get = true; // Устанавливаем флаг принятия погоды
    }
    return true;
}

// =======================================================================
// Проверка числа на .0 и отбрасывание его
// =======================================================================
String OWM_Lib::_CheckFloatNum(float CF_Num)
{
    String CF_str = String(CF_Num);
    ////Serial.println(CF_str+"    ===   "+String(CF_Num,0)+"    ===   "+String(CF_Num,1));
    (CF_str.indexOf(".0") > 0) ? CF_str = String(CF_Num, 0) : CF_str = String(CF_Num, 1);
    CF_str.trim();
    return CF_str;
}