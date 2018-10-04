{
INFO.PAS this file was extracted from 

https:/github.com/shidel/DustyTP7/blob/master/INFO/INFO.PAS

This file was used to adapt VIDEO.ASM.
}
{ 
Copyright 1990-2015, Jerome Shidel.
	
This project and related files are subject to the terms of the Mozilla Public License, 
v. 2.0. If a copy of the MPL was not distributed with this file, You can obtain one at 
http://mozilla.org/MPL/2.0/.
}


{$A+,B-,D+,E+,F-,G-,I+,L+,N-,O-,P-,Q-,R-,S+,T-,V+,X+,Y+}
{$M 16384,0,0}
program Information;

  uses Dos, StrAsm;

  const
    VESA    = 9;
    VGA     = 8;
    MCGA    = 7;
    EGA     = 6;
    CGA     = 5;
    InColor = 4;
    HGCPlus = 3;
    HGC     = 2;
    MDA     = 1;

  var
    VesaBuffer : array[0..255] of byte;
    Equipment : word;
    Is286     : boolean;

  function Is286Compatable: Boolean; assembler;
    asm
      PUSHF
      POP     BX
      AND     BX,0FFFH
      PUSH    BX
      POPF
      PUSHF
      POP     BX
      AND     BX,0F000H
      CMP     BX,0F000H
      MOV     AX,0
      JZ      @@1
      MOV     AX,1
    @@1:
      MOV     Is286, AL
  end;

  function DetectGraph : word; assembler;
    asm
    @@TestVesa:
      MOV  AX, SEG VesaBuffer
      MOV  ES, AX
      MOV  AX, OFFSET VesaBuffer
      MOV  DI, AX
      MOV  AH, $4f
      XOR  AL, AL
      INT  $10
      CMP  AX, $014F
      JNE  @@TestVGA
      MOV  AX, Vesa
      JMP  @@Done
    @@TestVGA:
      MOV AX, $1200
      MOV BL, $36
      INT $10
      CMP AL, $12
      JNE @@TestMCGA
      MOV AX, VGA
      JMP @@Done
    @@TestMCGA:
      MOV AX, $1200
      MOV BL, $32
      INT $10
      CMP AL, $12
      JNE @@TestEGA
      MOV AX, MCGA
      JMP @@Done
    @@TestEGA:
      MOV AX, $1200
      MOV BL, $10
      INT $10
      CMP BH, $10
      JNE @@TestCGA
      MOV AX, EGA
      JMP @@Done
    @@TestCGA:
      MOV  DX, $3D4
      MOV  AL, $0A
      OUT  DX, AL
      INC  DX
      IN   AL, DX
      XCHG AH, AL
      MOV  AL, 6
      OUT  DX, AL
      MOV  CX, $50
    @@TestCGA2:
      LOOP @@TestCGA2
      IN   AL, DX
      XCHG AH, AL
      OUT  DX, AL
      CMP  AH, 6
      JNE  @@TestMono
      MOV  AX, CGA
      JMP  @@Done
    @@TestMono:
      MOV  DX, $3B4
      MOV  AL, $0A
      OUT  DX, AL
      INC  DX
      IN   AL, DX
      XCHG AH, AL
      MOV  AL, 6
      OUT  DX, AL
      MOV  CX, $50
    @@TestMono2:
      LOOP @@TestMono2
      IN   AL, DX
      XCHG AH, AL
      OUT  DX, AL
      CMP  AH, 6
      JNE  @@UnknownDriver
      MOV  CX, 50000
      MOV  BX, $80
      XOR  AX, AX
      MOV  DX, $3BA
    @@TestMono3:
      IN     AL, DX
      TEST   AX, BX
      LOOPNZ @@TestMono3
      JZ     @@TestInColor
      MOV    AX, MDA
      JMP    @@Done
    @@TestInColor:
      AND    AL, 01110000b
      CMP    AL, 01010000b
      JNE    @@TestHGCPlus
      MOV    AX, InColor
      JMP    @@Done
    @@TestHGCPlus:
      CMP    AL, 00010000b
      JNE    @@TestHGC
      MOV    AX, HGCPlus
      JMP    @@Done
    @@TestHGC:
      MOV    AX, HGC
      JMP    @@Done
    @@UnknownDriver:
      MOV AX, 0
    @@Done:
    end;

  procedure Bar( Name : String );
    begin
      Name := ' ' + Name + ' ';
      While Length(Name) < 79 do Name := #176 + Name + #176;
      Name := Copy (Name, Length(Name) - 78, 79);
      WriteLn ( Name );
      WriteLn;
    end;

  procedure ComputerInfo;
    var
      ButtonCount : word;
      MouseAvail  : word;
      I           : Byte;
      Memory      : word;
      TPtr        : Pointer;
    begin
      Bar ('System');
      { Computer }
      Write ( 'Computer ID byte is ');
      Case Mem[$F000:$FFFE] of
        $FF : WriteLn('original PC.');
        $FE : WriteLn('XT or Portable PC.');
        $FD : WriteLn('PCjr.');
        $FC : WriteLn('AT (or XT model 286) (or PS/2 model 50/60).');
        $FB : WriteLn('XT with 640k motherboard.');
        $FA : WriteLn('PS/2 model 30.');
        $F9 : WriteLn('Convertable PC.');
        $F8 : WriteLn('PS/2 model 80.');
      else
        Writeln ( '???.' );
      end;
      Write ( 'CPU is ');
      if Not Is286 then Write ( 'not ');
      WriteLn ( 'a 286 compatable.');
      { ROM BIOS Date }
      Write ('ROM-BIOS release date of ');
      for I := 0 to 7 do
        Write ( Char(Mem[$F000:$FFF5 + I]) );
      WriteLn ('.');
      Write ('ROM-BASIC address is ');
      GetIntVec ( $18, TPtr );
      if TPtr = nil then
        WriteLn ('not assigned.')
      else
        WriteLn ('set to ', HexWord(SEG(TPtr^)), ':', HEXWord(Ofs(TPtr^)), '.' );
      WriteLn ( 'Programs load at ', HexWord(PrefixSeg), ':0000.');

      { Installed Equipment }
      WriteLn;
      Write ('Math coprocessor ');
      if Equipment and $02 = $02 then WriteLn ('is present.') else WriteLn ( 'was not found.');
      Write ('Game adapter ');
      if Equipment and $1000 = $1000 then WriteLn ('is present.') else WriteLn ( 'was not found.');
      Write ('PC internal modem ');
      if Equipment and $2000 = $2000 then WriteLn ('is present.') else WriteLn ( 'was not found.');
      { Mouse }
      Case Equipment and $c000 of
        $0000 : Write ('No printers');
        $4000 : Write ('1 printer is');
        $8000 : Write ('2 printers are');
        $C000 : Write ('3 printers are');
      end;
      WriteLn (' installed.');
      WriteLn;
      asm
        MOV AX, 0
        INT $33
        MOV MouseAvail, AX
        MOV ButtonCount, BX
      end;
      if MouseAvail = $FFFF then
        begin
          Write ( ButtonCount, ' button, ' );
          asm
            MOV AX, $24
            INT $33
            MOV MouseAvail,  BX
            MOV ButtonCount, CX
          end;
          Case Hi(ButtonCount) of
            1 : Write ('Bus');
            2 : Write ('Serial');
            3 : Write ('Inport');
            4 : Write ('PS/2');
            5 : Write ('HP');
          else
            Write('Type ', Hi(ButtonCount));
          end;
          Write (' mouse found at IRQ', Lo(ButtonCount), ', using driver version ');
          Write ( IntStr(Hi(MouseAvail)) + '.' + ZeroPad(IntStr(Lo(MouseAvail)), 2) );
          WriteLn ('.');
        end
      else
        WriteLn ('Mouse was not found.');
      WriteLn;
    end;

  procedure PortInfo;
    var
      I : byte;
      F : Boolean;
    begin
      Bar ('I/O Ports');
      if (Hi(Equipment) shr 1) and $07 = 0 then
        Write ( 'No' )
      else
        Write ( (Hi(Equipment) shr 1) and $07 );
      Write ( ' RS-232 port');
      if (Hi(Equipment) shr 1) and $07 <> 0 then Write ('s');
      WriteLn (' found.');
      F := False;
      for I := 0 to 3 do
        if MemW[Seg0040:I * Sizeof(Word)] <> 0 then
          begin
            if Not F then WriteLn;
            F := True;
            Write   ( 'COM', I + 1, ' base port is ');
            WriteLn ( HexWord(MemW[Seg0040:I * Sizeof(Word)]), 'H');
          end;
      F := False;
      for I := 0 to 3 do
        if MemW[Seg0040:$0008 + I * Sizeof(Word)] <> 0 then
          begin
            if Not F then WriteLn;
            F := True;
            Write   ( 'LPT', I + 1, ' base port is ');
            WriteLn ( HexWord(MemW[Seg0040:$0008 + I * Sizeof(Word)]), 'H');
          end;
      WriteLn;
      Write ('6859 video controller chip port is ');
      if MemW[Seg0040:$0063] = 0 then Write('not assigned') else
        Write ( HexWord(MemW[Seg0040:$0063]), 'H');
      WriteLn ('.');
      WriteLn;
    end;

  procedure VideoInfo;
    var
      Driver : word;
      TByte  : byte;
    begin
      Bar('Video');
      Write ( 'Initial video mode is ');
      Case Equipment and $30 of
        $00 : Write ( '???');
        $10 : Write ( '40x25 color');
        $20 : Write ( '80x25 color');
        $30 : Write ( 'monochrome');
      end;
      WriteLn ('.');
      Write ('Detected graphics driver is ');
      Driver := DetectGraph;
      Case Driver of
        MDA     : Write('MDA');
        HGC     : Write('HGC');
        HGCPlus : Write('HGC+');
        InColor : Write('InColor');
        CGA     : Write('CGA');
        EGA     : Write('EGA');
        MCGA    : Write('MCGA');
        VGA     : Write('VGA');
      else
        Write('unknown');
      end;
      WriteLn ('.');
      WriteLn;
      Write ('Current video mode is ', HexByte(Mem[Seg0040:$0049]), 'H means, ');
      Case Mem[Seg0040:$0049] of
        $0  : Write ('B&W yext 40x25');
        $1  : Write ('color text 40x25');
        $2  : Write ('B&W text 80x25');
        $3  : Write ('color text 80x25');
        $4, $5, $D, $13 : Write ('color graphics 320x200');
        $6, $E : Write ('color graphics 640x200');
        $7  : Write ('monochrome text 80x25');
        $F  : Write ('monochrome graphics 640x350');
        $10 : Write ('color graphics 640x350');
        $11, $12 : Write ('color graphics 640x480');
      else
        Write ('???');
      end;
      WriteLn('.');
      WriteLn ( 'Current visual video page is ', Mem[Seg0040:$0062], '.' );
      WriteLn ( 'Current video regen size is ', HexWord(MemW[Seg0040:$004c]), 'H.' );
      WriteLn ( 'Current total columns is ', MemW[Seg0040:$004a], '.' );
      if Driver >= EGA then
        begin
          WriteLn ( 'Current total rows is ', Mem[Seg0040:$0084] + 1, '.' );
          WriteLn ( 'Current character height is ', MemW[Seg0040:$0085], '.' );
          TByte := Mem[Seg0040:$087];
        end;
      if Driver >= EGA then
        begin
          WriteLn;
          WriteLn ('EGA specific information:');
          Write ( '  EGA cursor emulation is ');
          if TByte and $01 = $01 then WriteLn ('enabled.') else WriteLn ('disabled.');
          Write ( '  EGA is attached to ');
          if TByte and $02 = $02 then Write ('color') else Write ('monochrome');
          WriteLn (' display.');
          if TByte and $04 = $04 then
            WriteLn ('  EGA must wait for retrace.')
          else
            WriteLn ('  EGA does not have to wait for retrace.');
          Write ( '  EGA is ');
          if TByte and $08 = $08 then Write('not ');
          WriteLn ('active.');
          Write ('  EGA memory size is ');
          Case TByte and $60 of
            $00 : Write ( '64K');
            $20 : Write ( '128K');
            $40 : Write ( '192K');
            $60 : Write ( '256K+');
          end;
          WriteLn ('.');
          WriteLn ('  EGA dipswitch settings are ', Copy(BINByte(MEM[Seg0040:$0088]), 5, 4), '.' );
          WriteLn ('  EGA feature bits are ', Copy(BINByte(MEM[Seg0040:$0088]), 1, 4), '.' );
        end;
      WriteLn;
    end;

  procedure DriveInfo;
    var
      Regs : Registers;
      I    : byte;
      C    : byte;
      S    : String;
    begin
      Bar('Drives');
      if Equipment and $01 = $00 then
        WriteLn ( 'No floppy disk drives found.')
      else
        begin
          Write ( ((Equipment shr 6) and $03) + 1, ' floppy disk drive');
          if ((Equipment shr 6) and $03) + 1 <> 1 then Write ( 's');
          WriteLn (' found.');
        end;
      if Lo(DosVersion) >= 4 then
        with Regs do
          begin
            AX := $3305;
            Intr ( $21, Regs );
            WriteLn ( 'Boot drive was ', Char ( DL + 64 ), '.');
          end;
      if Lo(DosVersion) >= 3 then
        with Regs do
          begin
            C := 0;
            S := '';
            for I := 1 to 255 do
              begin
                AX := $4408;
                BL := I;
                Intr ( $21, Regs );
                if AX = $0F then Break
                else
                if AX = $00 then
                  begin
                    if C > 0 then S := S + ', ';
                    S := S + Char ( 64 + I );
                    Inc ( C );
                  end;
              end;
            if C = 0 then Write ( 'No d') else Write ('D');
            Write ( 'rive');
            if C <> 1 then Write ('s');
            Write (' using removable media');
            if C = 0 then WriteLn('.') else
            if C = 1 then WriteLn(' is ', S, '.' ) else
              WriteLn(' are ', S, '.' );
        end;
      if Lo(DosVersion) >= 3 then
        with Regs do
          begin
            C := 0;
            S := '';
            for I := 1 to 255 do
              begin
                AX := $4408;
                BL := I;
                Intr ( $21, Regs );
                if AX = $0F then Break
                else
                if AX = $01 then
                  begin
                    if C > 0 then S := S + ', ';
                    S := S + Char ( 64 + I );
                    Inc ( C );
                  end;
              end;
            if C = 0 then Write ( 'No f') else Write ('F');
            Write ( 'ixed disk drive');
            if C = 0 then WriteLn('s.') else
            if C = 1 then WriteLn(' is ', S, '.' ) else
              WriteLn('s are ', S, '.' );
        end;
      WriteLn;
    end;

  function EMMPresent : boolean;
    const
      Device : array[0..8] of Char = 'EMMXXXX0'#0;
    var
      I     : byte;
      Vec   : Pointer;
    begin
      GetIntVec ( $67, Vec );
      for I := 0 to Sizeof(Device) - 1 do
        if Device[I] <> Char(Mem[Seg(Vec^):Ofs(Vec^) + $0A + I]) then
          begin
            EMMPresent := False;
            Exit;
          end;
      asm
        MOV AH, $40
        INT $67
        MOV I, AH
      end;
      EMMPresent := I = 0;
    end;

  procedure MemoryInfo;
    var
      T, Memory, Free, Version : word;
    begin
      Bar ( 'Memory');
      Write ('Mother board memory is ');
      Case Equipment and $0c of
        $00 : Write ('16K');
        $04 : Write ('32K');
        $08 : Write ('48K');
        $0C : Write ('64K+');
      end;
      WriteLn ('.');
      asm
        INT $12
        MOV Memory, AX
      end;
      WriteLn ( 'Base memory is ', Memory, 'K.' );
      Write ( 'Expanded memory is ' );
      if Not EMMPresent then WriteLn ('not available.') else
        begin
          asm
            MOV AH, $42
            INT $67
            MOV T,      AX
            MOV Memory, DX
            MOV Free,   BX
            MOV AH, $46
            INT $67
            MOV Version, AX
          end;
          if Hi(T) = $00 then Write ( (LongInt(Memory) * 16), 'K') else Write ('ERROR');
          if Hi(Version) = $00 then Write ( ', EMM version ', (Version and $F0) Shr 4, '.', Version and $0F )  else
            Write (', ERROR');
          if Hi(T) = $00 then Write (', ', (LongInt(Free) * 16), 'K available') else Write (', ERROR');
          WriteLn ('.');
        end;
      WriteLn;
    end;

  procedure ENVInfo;
    var
      I : Word;
    begin
      if ENVCount > 0 then
        begin
          Bar ( 'Environment');
          For I := 1 to ENVCount do
            WriteLn ( ENVStr ( I ) );
          WriteLn;
        end;
    end;

  function Multiplex ( Proc : byte ) : Byte; assembler;
    asm
      MOV AH, Proc
      MOV AL, 0
      INT $2f
      JNC @@1
      MOV AL, $FF
      JMP @@Done
    @@1:
      CMP AL, $FF
      JNE @@2
      MOV AL, $02
    @@2:
      INC AL
    @@Done:
    end;

  procedure MultiplexStr ( T : Byte );
    begin
      T := Multiplex( T );
      if T = $FF then Write ( 'ERROR') else
        begin
          Write ( 'is ' );
          Case T of
            $01      : Write('not installed, and is OK to install');
            $02      : Write('not installed, and is not OK to install');
            $03, $81 : Write('installed');
          end;
        end;
      WriteLn ('.');
    end;

  procedure DOSInfo;
    var
      Tword : Word;
      TBool : Boolean;
      Regs  : Registers;
    begin
      BAR ('Dos');
      with Regs do
        begin
          AX := $3000;
          BX := 0;
          CX := 0;
          DX := 0;
          Intr ( $21, Regs );
          WriteLn ( 'DOS Version ', IntStr(AL), '.' , ZeroPad(IntStr(AH), 2) );
          WriteLn ( 'OEM serial number ', HexByte ( BH ), 'H' );
          WriteLn ( 'Software serial number ', HexByte ( BL ), HexWord ( CX ), 'H');
        end;
      Write ( 'Control Break checking is ');
      GetCBreak ( TBool );
      if TBool then WriteLn ('ON.') else WriteLn ('OFF.');
      Write ( 'Disk write verify is ');
      GetVerify ( TBool );
      if TBool then WriteLn ('ON.') else WriteLn ('OFF.');
      asm
        MOV AX, $3700
        INT $21
        MOV TWord, DX
      end;
      WriteLn( 'Switch character is "', Char(Lo(TWord)), '".' );
      if Lo(DosVersion) >= 3 then
        begin
          WriteLn;
          Write ('Print spooler ');
          MultiplexStr( $01 );
          Write ('Assign ');
          MultiplexStr( $02 );
          Write ('Share ');
          MultiplexStr( $10 );
          Write ('Append ');
          MultiplexStr( $10 );
          Write ('Himem ');
          MultiplexStr( $43 );
        end;
      WriteLn;
    end;

  var
    T : Word;

begin
  asm
    INT $11
    MOV Equipment, AX
  end;
  ComputerInfo;
  PortInfo;
  VideoInfo;
  DriveInfo;
  MemoryInfo;
  DOSInfo;
  ENVInfo;
end.
