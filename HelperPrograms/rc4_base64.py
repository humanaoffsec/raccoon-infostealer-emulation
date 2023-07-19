import base64
from Crypto.Cipher import ARC4
import sys

def encrypt_and_encode(input_string, key):
    cipher = ARC4.new(key)
    encrypted_data = cipher.encrypt(input_string)
    encoded_data = base64.b64encode(encrypted_data)
    return encoded_data.decode('utf-8')

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: python encrypt_and_encode.py [input_string]")
        sys.exit(1)
    
    input_string = sys.argv[1]
    key = b'arstarstarstarst'  # Replace with your own encryption key

    encrypted_and_encoded = encrypt_and_encode(input_string.encode('utf-8'), key)
    print("Encrypted and encoded string:", encrypted_and_encoded)