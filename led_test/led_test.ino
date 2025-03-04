#define DATA_PIN 11     // Пин данных
#define LATCH_PIN 10    // Пин защелки
#define CLOCK_PIN 8    // Пин тактов синхронизации

byte b[9] = {         // Байты, которые будут последовательно циклически выводиться в регистре
    0b00000000,
    0b00000001,
    0b00000011,
    0b00000111,
    0b00001111,
    0b00011111,
    0b00111111,
    0b01111111,
    0b11111111,
};

void setup() {
    pinMode(DATA_PIN, OUTPUT);    // Инициализация пинов
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    digitalWrite(LATCH_PIN, HIGH); // Установка начального состояния защелки

//    out_595_shift(0b11110000);
}

void loop() {
    static byte i = 0; // Индекс текущего байта
    out_595_shift(b[i]); // Передача байта на регистр
    i = (i == 9) ? 0 : i + 1; // Подготовка следующего байта
//    out_595_shift(0b11111111);
    delay(1000); // Задержка между установками 1 сек
}

void out_595_shift(byte x) {
    digitalWrite(LATCH_PIN, LOW); // "Открываем защелку"
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, x); // Отправляем данные
    digitalWrite(LATCH_PIN, HIGH); // "Закрываем защелку", выходные ножки регистра установлены
}
