void digitZero(int offset, int colour){
    stripClock.fill(colour, (0 + offset), 36);
    stripClock.fill(colour, (48 + offset), 36);
}

void digitOne(int offset, int colour){
    stripClock.fill(colour, (0 + offset), 12);
    stripClock.fill(colour, (48 + offset), 12);
}

void digitTwo(int offset, int colour){
    stripClock.fill(colour, (0 + offset), 24);
    stripClock.fill(colour, (36 + offset), 12);
    stripClock.fill(colour, (60 + offset), 24);
}

void digitThree(int offset, int colour){
    stripClock.fill(colour, (0 + offset), 24);
    stripClock.fill(colour, (36 + offset), 36);
}

void digitFour(int offset, int colour){
    stripClock.fill(colour, (0 + offset), 12);
    stripClock.fill(colour, (24 + offset), 36);
}

void digitFive(int offset, int colour){
    stripClock.fill(colour, (12 + offset), 60);
}

void digitSix(int offset, int colour){
    stripClock.fill(colour, (12 + offset), 72);
}

void digitSeven(int offset, int colour){
    stripClock.fill(colour, (0 + offset), 24);
    stripClock.fill(colour, (48 + offset), 12);
}

void digitEight(int offset, int colour){
    stripClock.fill(colour, (0 + offset), 84);
}

void digitNine(int offset, int colour){
    stripClock.fill(colour, (0 + offset), 72);
}
