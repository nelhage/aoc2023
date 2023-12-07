import argparse
import re

digits = dict(
  one=1,
  two=2,
  three=3,
  four=4,
  five=5,
  six=6,
  seven=7,
  eight=8,
  nine=9,
)

fwd_pat = "|".join(['[0-9]', *digits.keys()])
# bwd_pat = "|".join('[0-9]', *[d[::-1] for d in digits.keys()])

def one_line(line):
  first = re.search(fwd_pat, line).group(0)
  last = re.match(f".*({fwd_pat})", line).group(1)

  return (
    int(digits.get(first, first)) * 10
    + int(digits.get(last, last)))

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument("--input", default='test.txt')

  args = parser.parse_args()

  with open(args.input, 'r') as fh:
    lines = list(fh.readlines())

  # print(f"Read {len(lines)} lines.")
  calibration = 0
  for line in lines:
    calibration += one_line(line)

  print(calibration)

if __name__ == '__main__':
  main()
