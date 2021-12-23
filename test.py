import RPi.GPIO as GPIO
import sys
import signal
import time
from mingus.containers import *
from mingus.midi import fluidsynth

INPUT_PINS = (2, 3, 4)
PLAYBACK_ON = GPIO.LOW  # play on keypress, keypress is LOW state
PLAYBACK_OFF = GPIO.HIGH

# RPI pin to node mapping
NOTE_MAPPING = {
    2: Note('C-2', velocity=127),
    3: Note('E-2', velocity=127),
    4: Note('G-2', velocity=127),
}


class Organ:
    def __init__(self):
        self.gpio_setup()
        self.fluidsynth_setup()

        self.key_state = {
            2: GPIO.HIGH,
            3: GPIO.HIGH,
            4: GPIO.HIGH,
        }

        self.playback_state = {
            2: PLAYBACK_OFF,
            3: PLAYBACK_OFF,
            4: PLAYBACK_OFF,
        }

    def _callback(self, channel):
        input = GPIO.input(channel)
        # print(f'Button {channel} is {input}')
        self.key_state[channel] = input

    def gpio_setup(self):
        GPIO.setmode(GPIO.BCM)
        for pin in INPUT_PINS:
            GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

        for pin in INPUT_PINS:
            # GPIO.add_event_detect(pin, GPIO.BOTH, callback=self._callback, bouncetime=5)
            GPIO.add_event_detect(pin, GPIO.BOTH, callback=self._callback)

    def gpio_cleanup(self):
        GPIO.cleanup()

    def fluidsynth_setup(self):
        fluidsynth.init('Burea_Choir_2.01.SF2', 'alsa')
        fluidsynth.main_volume(1, 127)

    def fluidsynth_cleanup(self):
        fluidsynth.stop_everything()

    def cleanup(self):
        self.fluidsynth_cleanup()
        self.gpio_cleanup()

    def set_note(self, note, state):
        if state == PLAYBACK_ON:
            fluidsynth.play_Note(note)
        elif state == PLAYBACK_OFF:
            fluidsynth.stop_Note(note)

    # tohle smrdi race condition pri mackani tlacitek
    def play(self):
        for key, note in NOTE_MAPPING.items():
            if self.key_state[key] != self.playback_state[key]:
                self.playback_state[key] = self.key_state[key]
                self.set_note(note, self.playback_state[key])

    def __call__(self):
        counter = 0
        while True:
          counter += 1
          try:
              # if counter % 50 == 0:
              #   print(f'Key state: {self.key_state}')
              self.play()
              time.sleep(0.01)
          except KeyboardInterrupt:
              print("Exiting")
              self.cleanup()
              sys.exit(0)


if __name__ == '__main__':
    o = Organ()
    o()

