import library
import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *
import os

ROWS = 20
COLS = 10
Time=1000

IMAGES = {
    0: "nezapoln.jpeg",
    1: "zapoln.jpeg",
    2: "zapoln.jpeg",
}


class TetrisWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle('Tetris')
        self.installEventFilter(self)

        self.board = [[0] * COLS for _ in range(ROWS)]

        self.central_widget = QWidget(self)

        self.setCentralWidget(self.central_widget)

        self.layout = QVBoxLayout(self.central_widget)

        self.button_layout = QHBoxLayout()
        self.layout.addLayout(self.button_layout)

        self.button_reset = QPushButton()
        self.button_reset.setIcon(QIcon("icons/reset.png"))
        self.button_reset.setFixedSize(50, 50)
        self.button_reset.clicked.connect(self.button_reset_cliked)

        self.button_pause = QPushButton()
        self.button_pause.setIcon(QIcon("icons/pause.png"))
        self.button_pause.setFixedSize(50, 50)
        self.button_pause.clicked.connect(self.button_pause_clicked)

        self.button_layout.addWidget(self.button_reset)
        self.button_layout.addWidget(self.button_pause)

        self.grid_layout = QGridLayout()
        self.frame = QFrame()
        self.frame.setLayout(self.grid_layout)
        self.grid_layout.setSpacing(0)
        self.layout.addWidget(self.frame)

        self.game_over_frame = QFrame()
        self.game_over_frame.setFrameShape(QFrame.StyledPanel)
        game_over_label=QLabel(self.game_over_frame)
        pixmap=QPixmap("icons/gameover.png").scaled(225,450)
        game_over_label.setPixmap(pixmap)
        game_over_label.setScaledContents(True)
        self.layout.addWidget(self.game_over_frame)
        self.game_over_frame.hide()

        self.labels = []
        self.tetris = library.tetris()
        self.matrix = self.tetris.get_matrix()
        for row in range(ROWS):
            for col in range(COLS):
                label = QLabel()
                label.setAlignment(Qt.AlignCenter)
                # label.setFont(QFont('Arial', 20))
                label.setScaledContents(True)
                self.grid_layout.addWidget(label, row, col)
                self.labels.append(label)

        self.update_board()

        self.timer = QTimer()
        self.timer.timeout.connect(self.down_action)
        self.timer.start(Time)

        palette = QPalette()
        palette.setColor(QPalette.Background, Qt.gray)
        self.central_widget.setAutoFillBackground(True)
        self.central_widget.setPalette(palette)

        self.is_paused = False

    def update_board(self):
        for row in range(ROWS):
            for col in range(COLS):
                image_path = os.path.join("icons/", IMAGES[self.matrix[row][col]])
                image = QImage(image_path)
                pixmap = QPixmap.fromImage(image)
                self.labels[row * COLS + col].setPixmap(pixmap)

    def finish_game(self):
        self.frame.hide()
        self.game_over_frame.show()

    @pyqtSlot()
    def down_action(self):
        self.tetris.preprocess_action_str("down")
        if self.tetris.get_flag_finish_game() == 1:
            self.finish_game()
        self.matrix = self.tetris.get_matrix()
        self.update_board()

    @pyqtSlot()
    def button_reset_cliked(self):
        self.game_over_frame.hide()
        self.frame.show()
        self.timer.stop()
        self.tetris.preprocess_action_str("reset")
        self.matrix = self.tetris.get_matrix()
        self.update_board()
        self.timer.start(Time)

    @pyqtSlot()
    def button_pause_clicked(self):
        self.is_paused = not self.is_paused
        if self.is_paused:
            self.button_pause.setIcon(QIcon("icons/continue.png"))
            self.timer.stop()
        else:
            self.button_pause.setIcon(QIcon("icons/pause.png"))
            self.timer.start(Time)

    def eventFilter(self, obj, event):
        if event.type() == QEvent.KeyPress:
            if self.is_paused:
                return True
        return super().eventFilter(obj, event)

    def keyPressEvent(self, event):
        key = event.key()

        if key == Qt.Key_Down:
            self.tetris.preprocess_action_str("down")
            if (self.tetris.get_flag_finish_game() == 1):
                self.finish_game()
            self.matrix = self.tetris.get_matrix()
            self.update_board()
        elif key == Qt.Key_Left:
            self.tetris.preprocess_action_str("left")
            self.matrix = self.tetris.get_matrix()
            self.update_board()
        elif key == Qt.Key_Right:
            self.tetris.preprocess_action_str("right")
            self.matrix = self.tetris.get_matrix()
            self.update_board()
        elif key == Qt.Key_R:
            self.tetris.preprocess_action_str("rotate")
            self.matrix = self.tetris.get_matrix()
            self.update_board()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = TetrisWindow()
    window.show()
    sys.exit(app.exec_())
