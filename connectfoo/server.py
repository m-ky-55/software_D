from flask import Flask, request, jsonify, render_template
import subprocess
import threading

app = Flask(__name__)

process = None
lock = threading.Lock()


def read_output():
    output = ""
    while True:
        char = process.stdout.read(1)
        if not char:
            break
        output += char
        if output.endswith("> ") or output.endswith(": "):
            break
    return output


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/start")
def start():
    global process

    process = subprocess.Popen(
        ["./a.out"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        bufsize=0
    )

    return jsonify({"output": read_output()})


@app.route("/send", methods=["POST"])
def send():
    user_input = request.json["input"]

    process.stdin.write(user_input + "\n")
    process.stdin.flush()

    return jsonify({"output": read_output()})


if __name__ == "__main__":
    app.run(debug=True)
