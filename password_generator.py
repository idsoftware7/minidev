#!/usr/bin/env python3
# password_generator.py

import secrets
import string
import argparse

def generar_contraseña(longitud=16):
    """
    Genera una contraseña segura con letras, números y símbolos.
    """
    if longitud < 4:
        raise ValueError("La longitud mínima debe ser 4 para incluir todos los tipos de caracteres")

    # Caracteres posibles
    letras = string.ascii_letters       # a-zA-Z
    numeros = string.digits             # 0-9
    simbolos = string.punctuation       # !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~

    # Aseguramos que la contraseña tenga al menos un caracter de cada tipo
    contraseña = [
        secrets.choice(letras),
        secrets.choice(numeros),
        secrets.choice(simbolos),
    ]

    # Rellenamos el resto de la contraseña con una mezcla segura
    todos = letras + numeros + simbolos
    for _ in range(longitud - len(contraseña)):

        contraseña.append(secrets.choice(todos))

    # Mezclamos la contraseña para no dejar predecible el orden
    secrets.SystemRandom().shuffle(contraseña)
    return ''.join(contraseña)

def guardar_en_archivo(contraseña, archivo):
    """
    Guarda la contraseña en un archivo, añadiéndola al final.
    """
    with open(archivo, 'a') as f:
        f.write(contraseña + '\n')
    print(f"Contraseña guardada en {archivo}")

def main():
    parser = argparse.ArgumentParser(description="Generador de contraseñas seguras")
    parser.add_argument('-l', '--longitud', type=int, default=16, help="Longitud de la contraseña (mínimo 4)")
    parser.add_argument('-f', '--archivo', type=str, help="Archivo donde guardar la contraseña")
    args = parser.parse_args()

    try:
        contraseña = generar_contraseña(args.longitud)
        print(f"\nContraseña generada: {contraseña}\n")
        if args.archivo:
            guardar_en_archivo(contraseña, args.archivo)
    except ValueError as ve:
        print(f"Error: {ve}")

if __name__ == "__main__":
    main()