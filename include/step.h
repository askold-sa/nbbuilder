/**
 * Здесь описывается структура данных "шаг программы".
 * Шаг программы отражает акт информационного взаимодействия
 * программы с окружением (например, обращение к ресурсам,
 * системные вызовы и т.п.)
 * Единичная история программы представляет собой последовательность шагов.
 * Поведение программы - это набор историй.
 * Чтобы не зависеть от конкретной реализации шага программы 
 * (какие данные нужны и что в точности представляет
 * шаг в плане реализации), мы разделяем интерфейс и реализацию.
 */
#ifndef __STEP_H
#define __STEP_H

#include<iostream>
#include<string>
#include<vector>
#include<map>


using namespace std;

enum InitResult { OK, ERROR };

//////////////////////////////////////////////////////////////////////////////
// Интерфейсная часть
//////////////////////////////////////////////////////////////////////////////
class Step {
    public:
        Step() {}
        Step(string init_str) {}
        ~Step() {}

        virtual void set_init(string init_str) = 0;
        virtual string get_init() = 0;
        virtual void set_name(string name) = 0;
        virtual string get_name() = 0;
        virtual void label_on() = 0;
        virtual void label_off() = 0;
        virtual bool is_label() = 0;
        virtual void set_visname(string visname) = 0;
        virtual string get_visname() = 0;
        virtual bool operator== (Step&) = 0;
        virtual bool operator!= (Step&) = 0;

        virtual InitResult init() = 0;
};


//////////////////////////////////////////////////////////////////////////////
// Реализация
//////////////////////////////////////////////////////////////////////////////
class StepImpl : public Step {
    private:
        string init_str_;
        string name_;
        string visname_;
        bool flabel_;

    public:
        StepImpl();
        StepImpl(string init_str);
        ~StepImpl();

        void set_init(string init_str);
        string get_init();
        void set_name(string name);
        string get_name();
        void label_on();
        void label_off();
        bool is_label();
        void set_visname(string visname);
        string get_visname();
        bool operator== (Step&);
        bool operator!= (Step&);

        InitResult init();
};

#endif // __STEP_H
