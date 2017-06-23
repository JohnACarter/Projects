package project3;

import static org.junit.Assert.*;

import org.junit.Test;


public class MyArrayListTest {

	
	@Test
	public void testAddNull() {
		MyList list = new MyArrayList();
		assertEquals(false,list.add(null));
	}
	
	@Test
	public void testAddOnce() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Mal",10);
		assertTrue(list.add(p));
	}
	
	@Test
	public void testAddTwice() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Mal",10);
		Puppy p2 = new Puppy("Anara",11);
		assertTrue(list.add(p));
		assertTrue(list.add(p2));
	}
	
	@Test
	public void testAddGrowOnce() {
		MyList list = new MyArrayList();
		Puppy alice = new Puppy("Alice",5);
		Puppy batman = new Puppy("Batman",10);
		Puppy carlos = new Puppy("Carlos",15);
		assertTrue(list.add(alice));
		assertTrue(list.add(batman));
		assertTrue(list.add(carlos));
	}
	
	@Test
	public void testAddGrowALot() {
		MyList list = new MyArrayList();
		int rand = (int)(Math.random()*100)+20;
		for(int i = 0; i < rand; i++){
			assertTrue(list.add(new Puppy("p"+i,i)));
		}
	}
	
	@Test
	public void testInsertIndexOutOfBoundsPositive() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Mal",10);
		assertFalse(list.insert(1, p));
	}
	
	@Test
	public void testInsertIndexOutOfBoundsNegative() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Mal",10);
		assertFalse(list.insert(-1, p));
	}

	
	@Test
	public void testInsertNull() {
		MyList list = new MyArrayList();
		assertFalse(list.insert(0, null));
	}

	@Test
	public void testInsertEmpty() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Mal",10);
		assertTrue(list.insert(0, p));
	}
	
	@Test
	public void testInsertHead() {
		MyList list = new MyArrayList();
		Puppy alice = new Puppy("Alice",5);
		Puppy batman = new Puppy("Batman",10);
		list.add(alice);
		assertTrue(list.insert(0,batman));
	}
	
	@Test
	public void testInsertTail() {
		MyList list = new MyArrayList();
		Puppy alice = new Puppy("Alice",5);
		Puppy batman = new Puppy("Batman",10);
		list.add(alice);
		assertTrue(list.insert(1,batman));
	}
	
	@Test
	public void testInsertMid() {
		MyList list = new MyArrayList();
		Puppy alice = new Puppy("Alice",5);
		Puppy batman = new Puppy("Batman",10);
		Puppy carlos = new Puppy("Carlos",15);
		Puppy draco = new Puppy("Draco",20);
		list.add(alice);
		list.add(batman);
		list.add(carlos);
		assertTrue(list.insert(2, draco));
	}

	@Test
	public void testClear() {
		MyList list = new MyArrayList();
		int rand = (int)(Math.random()*100)+20;
		for(int i = 0; i < rand; i++){
			list.add(new Puppy("p"+i,i));
		}
		list.clear();
		assertEquals(0,list.size());
	}

	@Test
	public void testContains() {
		MyList list = new MyArrayList();
		Puppy alice = new Puppy("Alice",5);
		Puppy batman = new Puppy("Batman",10);
		Puppy carlos = new Puppy("Carlos",15);
		list.add(alice);
		list.add(batman);
		list.add(carlos);
		assertTrue(list.contains(new Puppy(alice)));
		assertTrue(list.contains(new Puppy(batman)));
		assertTrue(list.contains(new Puppy(carlos)));
	}
	
	@Test
	public void testGetEmpty() {
		MyList list = new MyArrayList();
		assertNull(list.get(0));
	}

	@Test
	public void testGetOutOfBoundsPositive() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Mal",10);
		list.add(p);
		assertNull(list.get(1));
	}
	
	@Test
	public void testGetOutOfBoundsNegative() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Mal",10);
		list.add(p);
		assertFalse(list.insert(-1, p));
	}
	
	@Test
	public void testGet() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Mal",10);
		list.add(p);
		assertNotNull(list.get(0));
		assertTrue((new Puppy(p)).equals(list.get(0)));
	}

	@Test
	public void testIndexOf() {
		MyList list = new MyArrayList();
		int rand = (int)(Math.random()*100)+20;
		Puppy[] puppyMill = new Puppy[rand];
		for(int i = 0; i < puppyMill.length; i++){
			puppyMill[i] = new Puppy("p"+i,i);
			list.add(new Puppy("p"+i,i));
		}
		int r = (int)(Math.random()*rand);
		assertEquals(r,list.indexOf(puppyMill[r]));
	}

	@Test
	public void testIsEmptyEmpty() {
		MyList list = new MyArrayList();
		assertTrue(list.isEmpty());
	}
	
	@Test
	public void testIsEmptyAddAndRemoveIndex() {
		MyList list = new MyArrayList();
		Puppy alice = new Puppy("Alice",5);
		Puppy batman = new Puppy("Batman",10);
		Puppy carlos = new Puppy("Carlos",15);
		list.add(alice);
		list.add(batman);
		list.add(carlos);
		list.remove(0);
		list.remove(0);
		list.remove(0);
		assertTrue(list.isEmpty());
	}
	
	@Test
	public void testIsEmptyAddAndClear() {
		MyList list = new MyArrayList();
		int rand = (int)(Math.random()*100)+20;
		for(int i = 0; i < rand; i++){
			list.add(new Puppy("p"+i,i));
		}
		list.clear();
		assertTrue(list.isEmpty());
	}
	
	@Test
	public void testIsEmptyFailureToAdd() {
		MyList list = new MyArrayList();
		assertFalse(list.add(null));
		assertTrue(list.isEmpty());
	}
	
	@Test
	public void testIsEmptyFailureToInsert() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Mal",10);
		assertFalse(list.insert(1,p));
		assertTrue(list.isEmpty());
	}
	
	@Test
	public void testIsEmptyAddAndRemoveObject() {
		MyList list = new MyArrayList();
		Puppy alice = new Puppy("Alice",5);
		Puppy batman = new Puppy("Batman",10);
		Puppy carlos = new Puppy("Carlos",15);
		list.add(alice);
		list.add(batman);
		list.add(carlos);
		list.remove(alice);
		list.remove(batman);
		list.remove(carlos);
		assertTrue(list.isEmpty());
	}
	
	
	@Test
	public void testIsNotEmpty() {
		MyList list = new MyArrayList();
	    list.add(new Puppy("pup",12));
		assertFalse(list.isEmpty());
	}
	
	@Test
	public void testIsFarFromEmpty() {
		MyList list = new MyArrayList();
		int rand = (int)(Math.random()*100)+20;
		for(int i = 0; i < rand; i++){
			list.add(new Puppy("p"+i,i));
		}
		assertFalse(list.isEmpty());
	}

	@Test
	public void testRemoveIntEmpty() {
		MyList list = new MyArrayList();
		assertNull(list.remove(0));
	}
	
	@Test
	public void testRemoveIntOutOfBoundsPositive() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Jayne",9001);
		list.add(p);
		assertNull(list.remove(1));
	}
	
	@Test
	public void testRemoveIntOutOfBoundsNegative() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Jayne",9001);
		list.add(p);
		assertNull(list.remove(-1));
	}
	
	@Test
	public void testRemoveIntSuccess() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Jayne",9001);
		Puppy p2 = new Puppy("Kaylee",1337);
		list.add(p);
		list.add(p2);
		int prelim = list.size();
		Puppy result = (Puppy)list.remove(1);
		int aftermath = list.size();
		assertNotNull(result);
		assertTrue(prelim > aftermath);
		assertTrue(result.equals(new Puppy(p2)));
		assertFalse(list.contains(result));
	}

	
	@Test
	public void testRemoveObjectEmpty() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Jayne",9001);
		assertFalse(list.remove(p));
	}
	
	@Test
	public void testRemoveObjectNull() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Jayne",9001);
		list.add(p);
		assertFalse(list.remove(null));
	}
	
	@Test
	public void testRemoveObjectSuccess() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Jayne",9001);
		Puppy p2 = new Puppy("Kaylee",1337);
		list.add(p);
		list.add(p2);
		int prelim = list.size();
		boolean result = list.remove(new Puppy(p2));
		int aftermath = list.size();
		assertTrue(prelim > aftermath);
		assertTrue(result);
		assertFalse(list.contains(new Puppy(p2)));
	}
		
	@Test
	public void testSetOutOfBoundsPositive() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Jayne",9001);
		list.add(p);
		Puppy p3 = new Puppy("Kaylee",1337);
		Puppy p2 = (Puppy)list.get(1);
		list.set(1,p3);
		assertEquals(p2,list.get(1));
	}
	
	@Test
	public void testSetOutOfBoundsNegative() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Jayne",9001);
		list.add(p);
		Puppy p3 = new Puppy("Kaylee",1337);
		Puppy p2 = (Puppy)list.get(-1);
		list.set(-1,p3);
		assertEquals(p2,list.get(-1));
	}
	
	@Test
	public void testSetNull() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Jayne",9001);
		list.add(p);
		Puppy p2 = (Puppy)list.get(0);
		list.set(0,null);
		assertEquals(p2,list.get(0));
	}
	
	@Test
	public void testSetEmpty() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Jayne",9001);
		Puppy p2 = (Puppy)list.get(0);
		list.set(0,p);
		assertEquals(p2,list.get(0));
	}
	
	@Test
	public void testSetSuccess() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Jayne",9001);
		list.add(p);
		Puppy p3 = new Puppy("Kaylee",1337);
		Puppy p2 = (Puppy)list.get(0);
		list.set(0,p3);
		assertEquals(p3,list.get(0));
		assertNotEquals(p2,list.get(0));
	}

	@Test
	public void testSizeNone() {
		MyList list = new MyArrayList();
		assertEquals(0,list.size());
	}
	
	@Test
	public void testSizeOne() {
		MyList list = new MyArrayList();
		Puppy p = new Puppy("Jayne",9001);
		list.add(p);
		assertEquals(1,list.size());
	}
	
	@Test
	public void testSizeABunch() {
		MyList list = new MyArrayList();
		int rand = (int)(Math.random()*100)+20;
		for(int i = 0; i < rand; i++){
			list.add(new Puppy("p"+i,i));
		}
		assertEquals(rand,list.size());
	}
	
	@Test
	public void testSizeAddAndRemove() {
		MyList list = new MyArrayList();
		int rand = (int)(Math.random()*100)+20;
		for(int i = 0; i < rand; i++){
			list.add(new Puppy("p"+i,i));
		}
		list.remove(0);
		list.remove(0);
		assertEquals(rand-2,list.size());
	}
	
	@Test
	public void testSizeAddAndClear() {
		MyList list = new MyArrayList();
		int rand = (int)(Math.random()*100)+20;
		for(int i = 0; i < rand; i++){
			list.add(new Puppy("p"+i,i));
		}
		list.clear();
		assertEquals(0,list.size());
	}

}
