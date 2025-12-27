import hashlib, bencodepy

torrent = open("Whiplash.torrent", "rb").read()
decoded = bencodepy.decode(torrent)
info = bencodepy.encode(decoded[b'info'])
print(hashlib.sha1(info).hexdigest())
