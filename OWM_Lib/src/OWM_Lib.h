#pragma once
#include <Arduino.h>

class OWM_Lib
{
public:
  OWM_Lib(String lat, String lon, String appid); //настройка параметров
  void convWeatherRealDay();                     //Расшифровка погоды текущей и суточной в строку OWM.Weather_Tape с сайта openweathermap.org
  String Telegram_string();                      //строка погоды для телеграмм
  String Scroll_tape();                          //строка погоды для бегущей строки
  uint8_t get_Weather();                         // каждые 15 мин получаем данные текущей погоды и каждый час прогноз погоды

  struct OWM_Real_st
  {
    float coord_lon;            // 37.66,  !!!!!!!!! х100
    float coord_lat;            // 55.87  !!!!!!!!! х100
    uint16_t weather_id;        // 804,
    String weather_main;        // "Clouds",
    String weather_description; // "пасмурно",
    String weather_icon;        // "04n"
    float temp;                 // 0.27,    !!!!!!!!!! х10
    float feels_like;           // -5.81,    !!!!!!!!!! х10
    float temp_min;             // -0.56,    !!!!!!!!!! х10
    float temp_max;             // 1,    !!!!!!!!!! х10
    uint16_t pressure;          // 999,
    uint8_t humidity;           // 69
    uint16_t visibility;        // 10000,
    float wind_speed;           // 5,
    uint16_t wind_deg;          // 240
    uint8_t clouds_all;         //92
    float rain_1h;              //x10 Объем дождя за последний час, мм
    float snow_1h;              //x10 Объем снега за 1 час, мм
    time_t sunrise;             // 1582863915,
    time_t sunset;              // 1582901951
    uint32_t dt;                //1599569691 Время расчета данных, unix, UTC
    uint32_t city_id;           // 527981,
    String name;                // "Медведково",
    uint32_t Synchr_www;        // Время через сколько будет сихронизация текущей погоды с инетом
    String str_Rain;            // строка для дождя
    String str_Wind;            // описание текущего ветра для web servera
    bool fl_Get : 1;            // Флаг принятие текущей погоды
                                /*
    float rain_3h;              //x10 Объем дождя за последние 3 часа, мм
    float snow_3h;              //x10 Объем снега за последние 3 часа, мм*/
  };

  struct OWM_Day_st
  {
    time_t dt;                  //Время прогнозирования данных, Unix, UTC
    float main_temp;            //Температура_ Единица измерения по умолчанию: Кельвин, Метрическая система: Цельсий, Имперская система: Фаренгейт_
    float main_feels_like;      //Этот температурный параметр объясняет человеческое восприятие погоды_ Единица измерения по умолчанию: Кельвин, Метрическая система: Цельсий, Имперская система: Фаренгейт_
    float main_temp_min;        //Минимальная температура на момент расчета_ Это минимальная прогнозируемая температура (в пределах крупных мегаполисов и городских территорий), используйте этот параметр по желанию_ Единица измерения по умолчанию: Кельвин, Метрическая система: Цельсий, Имперская система: Фаренгейт_
    float main_temp_max;        //Максимальная температура на момент расчета_ Это максимальная прогнозируемая температура (в пределах крупных мегаполисов и городских территорий), используйте этот параметр по желанию_ Единица измерения по умолчанию: Кельвин, Метрическая система: Цельсий, Имперская система: Фаренгейт_
    uint16_t main_pressure;     //Атмосферное давление на уровне моря по умолчанию, гПа
    uint16_t main_sea_level;    //Атмосферное давление на уровне моря, гПа
    uint16_t main_grnd_level;   //Атмосферное давление на уровне земли, гПа
    uint8_t main_humidity;      //Влажность,%
    float main_temp_kf;         //Внутренний параметр
    uint16_t weather_id;        //Идентификатор погодных условий
    String weather_main;        //Группа погодных параметров (Дождь, Снег, Экстрим и др_)
    String weather_description; //Погодные условия в группе_ Вы можете получить вывод на своем языке_ Узнать больше _
    String weather_icon;        //Идентификатор значка погоды
    uint8_t clouds_all;         //Облачность,%
    float wind_speed;           //Скорость ветра_ Единица измерения по умолчанию: метр / сек, метрическая система: метр / сек, британская система мер: мили / час_
    uint16_t wind_deg;          //Направление ветра, градусы (метеорологические)
    float rain_3h;              //Объем дождя за последний час, мм
    float snow_3h;              //Объем снега за последний час
    uint16_t visibility;        //Средняя видимость, метры
    uint8_t pop;                //Вероятность выпадения осадков
    String sys_pod;             //Часть дня (n - ночь, d - день)
    String dt_txt;              //Время прогнозирования данных, ISO, UTC
    uint16_t city_id;           //Идентификатор города (доступен, если вызов API был выполнен с помощью идентификатора города )
    String city_name;           //Название города
    float city_coord_lat;       //Географическое положение города, широта
    float city_coord_lon;       //Географическое положение города, долгота
    String country;             //Код страны (Великобритания, Япония и т_ Д_)
    uint32_t timezone;          //Сдвиг в секундах от UTC
    time_t sunrise;             //Время восхода, Unix, UTC
    time_t sunset;              //Время заката, Unix, UTC
    uint32_t Synchr_www;        // Время через сколько будет сихронизация прогноза погоды с инетом
    String str_Rain;            //строка для дождя
    String str_Wind;            // описание прогноза ветра для web server
    bool fl_Get : 1;            // Флаг принятие суточной погоды
    time_t Time_Weather_Sun_Up; // Время восхода солнца
    time_t Time_Weather_Sun_Dw; // Время заката солнца
  };

  struct Graph_Data_st
  {
    int16_t Temperature_GR;      // данные для графика температуры
    int16_t Temperature_Feel_GR; // данные для графика температуры
    uint8_t pressure_GR;         // данные для графика давления
    uint8_t humidity_GR;         // данные для графика влажности
    uint32_t Graphic_Time;       // Время для графика температуры
    uint16_t home_ligth;         // данные для графика освещенности
  };

  OWM_Real_st Real;             // текущая погода
  OWM_Day_st Day;               // прогноз погоды
  Graph_Data_st Graph_Data[98]; //данные для графиков
  uint8_t Err_WiFi_Connect;     // число попыток получения погоды
  time_t NTP_Local;             // Время NTP на контроллере
  enum Errors
  {
    Real_OK,
    Get_Real,
    Day_OK,
    Get_Day,
    No_synhr
  };

private:
  String _CheckFloatNum(float CF_Num);                // Проверка числа на .0 и отбрасывание его
  String _Tbot_Weather_string = "";                   // Строка погоды для Telegramm bot
  String _Scroll_tape;                                //Строка с погодой
  String _lat = "55.866453";                          // latitude
  String _lon = "37.662229";                          //longtitude
  String _appid = "74d56d846843243e70f740ff0611a63d"; // token OWM

  bool _get_Weather(bool Real_Day); //чтение с сервера погоды
};
