public class SimPractice {
    public static PQ agenda;

    public static void main(String[] args) {
        agenda = new PQ();
        agenda.add(new Event1(), 1);
        agenda.add(new Event2(), 10);
        agenda.add(new Event4(), 15);

        while (agenda.getCurrentTime() < 100) {
            agenda.remove().run();
        }
    }
}
