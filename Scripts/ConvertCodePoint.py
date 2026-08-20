def utf8_escape_for_codepoint(cp: int) -> str:
    b = chr(cp).encode("utf-8")
    return '"' + "".join(f"\\x{byte:02x}" for byte in b) + '"'

def parse_codepoint(s: str) -> int:
    s = s.strip()
    if not s:
        raise ValueError("Empty input")

    # s_upper = s.upper()
    # if s_upper.startswith("U+"):
    #     s_hex = s[2:]
    # elif s_upper.startswith("0X"):
    #     s_hex = s[2:]
    # else:
    #     s_hex = s

    return int(s[2:], 16)

def main():
    while True:
        try:
            user_in = input("> ").strip()
            if not user_in:
                continue
            if user_in.lower() in {"q", "quit", "exit"}:
                break

            cp = parse_codepoint(user_in)
            s = utf8_escape_for_codepoint(cp)

            print(f"{s}\t// U+{cp:04X}\n")

        except ValueError as e:
            print(f"Error: {e}\n")
        except KeyboardInterrupt:
            print("\nBye.")
            break

if __name__ == "__main__":
    main()
