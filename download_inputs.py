import requests
import os
import datetime

session = os.environ.get('SESSION')


if session is None:
    print("Please set the SESSION environment variable.")
    exit(1)


headers = { "Cookie": f"session={session}" }

now = datetime.datetime.now()

if now > datetime.datetime(2024, 12, 25):
    days = 25

else:
    days = now.day




for i in range(1, days+1):
    url = f"https://adventofcode.com/2024/day/{i}/input"
    filename = f"inputs/{i:02}.txt"

    if os.path.exists(filename):
        print(f"Skipping {filename}")
        continue

    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        with open(filename, 'wb') as f:
            f.write(response.content)
