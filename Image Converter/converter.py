import os
import sys
from PIL import Image

PALETTE = [
    (0, 0, 0), (255, 255, 255), (67, 138, 28),
    (100, 64, 255), (191, 0, 0), (255, 243, 56),
    (232, 126, 0), (194, 164, 244)
]

def quantize_image_to_palette(img, palette):
    """Quantize an image to a specific palette using Floyd-Steinberg dithering."""
    img = img.convert("RGB")
    pixels = img.load()
    width, height = img.size

    for y in range(height):
        for x in range(width):
            old_pixel = img.getpixel((x, y))
            new_pixel = closest_colour(old_pixel, palette)
            pixels[x, y] = new_pixel

            quant_error = tuple(oe - ne for oe, ne in zip(old_pixel, new_pixel))

            # Distribute the quantization error to neighboring pixels
            distribute_error(img, x + 1, y, quant_error, 7 / 16)
            distribute_error(img, x - 1, y + 1, quant_error, 3 / 16)
            distribute_error(img, x, y + 1, quant_error, 5 / 16)
            distribute_error(img, x + 1, y + 1, quant_error, 1 / 16)

    return img

def distribute_error(img, x, y, error, coefficient):
    """Distribute quantization error to a specific pixel."""
    if 0 <= x < img.width and 0 <= y < img.height:
        pixel = img.getpixel((x, y))
        modified_pixel = tuple(int(pixel[i] + error[i] * coefficient) for i in range(3))
        img.putpixel((x, y), modified_pixel)

def closest_colour(rgb, palette):
    """Get the closest colour from the palette to a given RGB value."""
    return min(palette, key=lambda colour: sum(abs(c - rc) for c, rc in zip(colour, rgb)))

def convert_image_to_array(img, palette):
    """Convert the image to a byte array format using the palette."""
    pixels = list(img.getdata())
    colour_to_byte = {colour: index for index, colour in enumerate(palette)}
    
    byte_array = []
    for i in range(0, len(pixels), 2):
        pixel1 = colour_to_byte[pixels[i]]
        pixel2 = colour_to_byte[pixels[i+1] if i+1 < len(pixels) else PALETTE[-1]]  # Default to last palette colour if out of range
        packed_byte = (pixel1 << 4) + pixel2
        byte_array.append(packed_byte)

    return byte_array

def apply_gamma_correction(img, gamma):
    """Apply gamma correction to the image."""
    gamma_inv = 1.0 / gamma
    gamma_table = [min(int((i / 255.0) ** gamma_inv * 255.0), 255) for i in range(256)]
    if img.mode == "RGB":
        gamma_table *= 3
    return img.point(gamma_table)

def generate_c_file(image_path, output_dir):
    """Generate C and H files from an image."""
    img = Image.open(image_path).resize((800, 480))
    img = apply_gamma_correction(img, 1.2)  # Adjust the gamma value (1.2) as needed
    img = quantize_image_to_palette(img, PALETTE)
    byte_array = convert_image_to_array(img, PALETTE)

    base_filename = os.path.splitext(os.path.basename(image_path))[0]
    c_filename, h_filename = base_filename + ".c", base_filename + ".h"
    array_name = f"{base_filename}Image7colour"

    with open(os.path.join(output_dir, c_filename), 'w') as f:
        f.write(f'#include "{h_filename}"\n\n')
        f.write(f"const unsigned char {array_name}[192000] = {{")
        for i, byte in enumerate(byte_array):
            if i % 16 == 0:
                f.write("\n")
            f.write(f"0x{byte:02x},")
        f.write("\n};\n")

    with open(os.path.join(output_dir, h_filename), 'w') as f:
        f.write(f"#ifndef _{base_filename.upper()}_H_\n")
        f.write(f"#define _{base_filename.upper()}_H_\n\n")
        f.write(f"extern const unsigned char {base_filename}Image7colour[];\n\n")
        f.write("#endif\n")

    print(f"Generated: {os.path.join(output_dir, c_filename)}")
    print(f"Generated: {os.path.join(output_dir, h_filename)}")

def main(input_dir, output_dir):
    """Main processing function."""
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    for filename in os.listdir(input_dir):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg')):
            generate_c_file(os.path.join(input_dir, filename), output_dir)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script_name.py <input_directory> <output_directory>")
        sys.exit(1)
    main(sys.argv[1], sys.argv[2])
