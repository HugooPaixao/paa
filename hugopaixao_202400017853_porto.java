import java.io.*;

class Conteiner{
  private String cod;
  private String cnpj;
  private double peso;
  private int pos;
  private int prio;
  private int difPeso;
  private double difPercentual;
  private String cnpjCad;

  public Conteiner(String cod, String cnpj, double peso, int pos) { // talvez depois tirar o pos daqui, vejo depois
    this.cnpj = cnpj;
    this.cod = cod;
    this.peso= peso;
    this.pos = pos;
    this.prio = 0;
  }

  public String getCnpj() {
    return cnpj;
  }

  public String getCod() {
    return cod;
  }

  public double getPeso() {
    return peso;
  }

  public int getPos() {
    return pos;
  }

  public int getPrio() {
    return prio;
  }

  public int getDifPeso() {
    return difPeso;
  }

  public double getDifPercentual() {
    return difPercentual;
  }

  public String getCnpjCad() {
    return cnpjCad;
  }

  public void setCod(String cod) {
    this.cod = cod;
  }

  public void setPos(int pos) {
    this.pos = pos;
  }

  public void setCnpjCad(String cnpjCad) { //verifica se possui divergencia de cnpj, prioriadade 1
    this.prio = 1;
    this.cnpjCad = cnpjCad;
  }

  public void setDifPeso(int difPeso, double difPercentual) { // verifica se possui devergencia de peso, prioriade 2
    this.prio = 2;
    this.difPeso = difPeso;
    this.difPercentual = difPercentual;
  }

  public String exibir() {
    if(prio == 1)
      return cod+":"+cnpjCad+"<->"+cnpj;

    else if(prio == 2)
      return cod+":"+(int)difPeso+"kg("+(int)difPercentual+"%)";

    return "";
  }
}

class MergeSortConteineres{

  private boolean ordenaCod = false;   // se estiver true ordena por codigo, senao por prioridade

  public void ordenarPorCod(boolean ordenaCod) {
    this.ordenaCod = ordenaCod;
  }

  public void sort(Conteiner[] v, int qtd) {
    if(qtd > 0) {
      Conteiner[] w = new Conteiner[qtd];
      mergeSort(v, w,0, qtd-1);
    }
  }

  public void mergeSort(Conteiner[] v, Conteiner[] w, int inicio, int fim) {
    if(inicio < fim) {
      int meio = (inicio+fim)/2;

      mergeSort(v, w, inicio, meio);
      mergeSort(v, w, meio+1, fim);
      intercalar(v, w,inicio, meio, fim);
    }
  }

  public int comp(Conteiner a, Conteiner b) {
    if(ordenaCod == true)
      return a.getCod().compareTo(b.getCod());

    if(a.getPrio() != b.getPrio())
      return a.getPrio() - b.getPrio(); // se a prio for diferente organiza pela maior prio, claro

    if(a.getPrio() == 2) {
      int comp = Double.compare(b.getDifPercentual(), a.getDifPercentual());  // se a prio for 2, organiza pela dif percentual menor pra maior

      if(comp != 0)
        return comp;
    }
    return a.getPos() - b.getPos(); // se prio for igual organiza pela ordem de cadastro
  }

  public void intercalar(Conteiner[] v, Conteiner[] w, int inicio, int meio, int fim) {
    for(int k = inicio; k <= fim; k++)
      w[k] = v[k];

      int i = inicio;
      int j = meio+1;

      for(int k = inicio; k <= fim; k++) {
        if(i > meio) v[k] = w[j++];
        else if(j > fim) v[k] = w[i++];
        else if(comp(w[i], w[j]) <= 0) v[k] = w[i++];
        else v[k] = w[j++];
      }
  }
}

class Fiscalizar{

  public int buscaBin(Conteiner[] cad, String cod) { // busca binaria por codigo
    int inicio = 0;
    int fim = cad.length-1;

    while(inicio <= fim) {
      int meio = (inicio+fim)/2;
      int comparar = cad[meio].getCod().compareTo(cod); // compara o codigo sel com o cad

      if(comparar == 0)
        return meio;
      else if(comparar > 0)
        fim = meio-1;
      else
        inicio = meio+1;
    }
    return -1;
  }

  public Conteiner[] fiscalizar(Conteiner[] cad, Conteiner[] sel) {
    int divCont = 0;
    Conteiner[] div = new Conteiner[sel.length];  // a qtd de conteiners divergentes eh igual ao tamanho dos selecionados

    for(Conteiner s: sel) {
      int comp = buscaBin(cad, s.getCod());

      if(comp != -1) {

        Conteiner c = cad[comp];
        s.setPos(c.getPos());

        if(!s.getCnpj().equals(c.getCnpj())) {
          s.setCnpjCad(c.getCnpj());
          div[divCont++] =  s;
          continue;
        }

        double difPeso = Math.abs(s.getPeso()-c.getPeso());
        double difPercentual = Math.round((difPeso*100.0)/c.getPeso());

        if(difPercentual > 10.0) {
          s.setDifPeso((int)difPeso, difPercentual);
          div[divCont++] = s;
        }
      }
    }

    Conteiner[] fiscalizado = new Conteiner[divCont];

    for(int i = 0; i < divCont; i++) {
      fiscalizado[i] = div[i];
    }
    return fiscalizado;
  }
}

public class hugopaixao_202400017853_porto {
  public static void main(String[] args) {
    try {
      System.out.println("#ARGS = " + args.length);
      System.out.println("ARG1 = " + args[0] + ", ARG2 = " + args[1]);

      BufferedReader in = new BufferedReader(new FileReader(args[0]));
      BufferedWriter out = new BufferedWriter(new FileWriter(args[1]));

      int cads = Integer.parseInt(in.readLine().trim());
      Conteiner[] cad = new Conteiner[cads];

      for (int i = 0; i < cads; i++) {
        String linha = in.readLine().trim();
        String[] partes = linha.split(" ");
        String cod = partes[0];
        String cnpj = partes[1];
        double peso = Double.parseDouble(partes[2]);
        cad[i] = new Conteiner(cod, cnpj, peso, i);
      }

      int sels = Integer.parseInt(in.readLine().trim());
      Conteiner[] sel = new Conteiner[sels];

      for (int i = 0; i < sels; i++) {
        String linha = in.readLine().trim();
        String[] partes = linha.split(" ");
        String cod = partes[0];
        String cnpj = partes[1];
        double peso = Double.parseDouble(partes[2]);
        sel[i] = new Conteiner(cod, cnpj, peso, i);
      }

      MergeSortConteineres mergeSortCod = new MergeSortConteineres();
      mergeSortCod.ordenarPorCod(true);
      mergeSortCod.sort(cad, cad.length); // ordenando por codigo

      Fiscalizar f = new Fiscalizar();
      Conteiner[] div = f.fiscalizar(cad, sel);

      MergeSortConteineres mergeSort = new MergeSortConteineres();
      mergeSort.ordenarPorCod(false);
      mergeSort.sort(div, div.length);

      for (int i = 0; i < div.length; i++) {
        out.write(div[i].exibir());
        out.newLine();
      }

      in.close();
      out.close();
    }
    catch (Exception e) {
      e.printStackTrace();
    }
  }
}
