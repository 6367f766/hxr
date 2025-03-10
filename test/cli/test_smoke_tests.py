import subprocess
import pytest
import os


class CommandResult:
    def __init__(self, result):
        self.stdout = result.stdout
        self.stderr = result.stderr


def run_command(command, *args) -> CommandResult:
    result = subprocess.run([command, *args], capture_output=True, text=True)
    return CommandResult(result)


def test_release_package_is_built():
    # NOTE: this will work as long as pytest is always executed from root dir
    assert "main" in os.listdir("build_outputs/release/main/")


class BetterVisual:
    def __init__(self, string, format_values):
        self.string = string
        self.format_values = format_values
        self.new_string = ""
        self.format()

    def get_next(self):
        for i in self.format_values:
            num = ""
            for ch in i:
                if ch.isnumeric():
                    num += ch
                else:
                    break

            num = eval(num)
            for j in range(num):
                yield i.replace(str(num), "")

    def format(self):
        values = self.string.split("-")
        if self.string.startswith("-"):
            values = values[1:]

        values_to_format = list(self.get_next())

        # to debug:
        for (ind, val) in enumerate(values):
            self.new_string += values_to_format[ind] + val

    def get(self):
        return self.new_string


# -64-00-00-

@pytest.mark.parametrize("test_input,expected",
                         [
                             (["query", "-d", "-100"], BetterVisual("-9c-ff-ff-ff-",
                              ["1   ", "3\t\t", "1\t\t\n"]).get()),
                             (["query", "-bu", "100"], BetterVisual("-64-00-00-00-0110 0100----",
                              ["1   ", "3\t\t", "1\t\t\n", "3\t0000 0000", "1\t\n"]).get()),
                             (["query", "-x", "ff"],
                              BetterVisual("-255-", ["1", "1\n"]).get()),
                             (["query", "-xb", "ff"], BetterVisual("-255-1111 1111----",
                              ["1", "1\n", "3\t0000 0000", "1\t\n"]).get()),
                             (["query", "-t", "hello"], BetterVisual("-h-e-l-l-o-68-65-6c-6c-6f-0110 1000-0110 0101-0110 1100-0110 1100-0110 1111-", [
                              "1", "4\t\t", "1\t\t\n", "4\t\t", "1\t\t\n", "4\t", "1\t\n"]).get()),
                         ])
def test_cli_default(test_input, expected):
    result = run_command("build_outputs/release/main/main", *test_input).stdout
    assert run_command("build_outputs/release/main/main",
                       *test_input).stdout == expected
