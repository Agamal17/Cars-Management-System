import sys
import requests
from bs4 import BeautifulSoup

def main():
    search_keyword = sys.argv[1]
    download_path = r"../images\\"  # Set the path where you want to save the image

    url = f"https://www.gettyimages.com/photos/{search_keyword}"
    headers = {"User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36"}
    response = requests.get(url, headers=headers)
    soup = BeautifulSoup(response.content, "html.parser")

    # Find the first image link (adjust selector if needed)
    image_link = soup.find("img", class_ = 'BLA_wBUJrga_SkfJ8won')['src']

    if image_link:
        with open(download_path+search_keyword+'.jpg', "wb") as file:
            response = requests.get(image_link, stream=True)
            for chunk in response.iter_content(1024):
                file.write(chunk)

main()
