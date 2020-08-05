RSpec.describe Annoy do
  it "has a version number" do
    expect(Annoy::VERSION).not_to be nil
  end

  it "does something useful" do
    #expect(false).to eq(true)
    a = Annoy::AnnoyIndex.new(n_features: 3)
    #a = Annoy::AnnoyIndexEuclidean.new(3)
    #a = Annoy::AnnoyIndexHamming.new(3)
    p a
    p a.verbose(true)
    p a.set_seed(1)
    p a.add_item(0, [1, 2, 3])
    p a.add_item(1, [2, 5, 4])
    p a.add_item(2, [0, 3, 1])
    #p a.add_item(0, [1, 0, 1])
    #p a.add_item(1, [0, 1, 1])
    #p a.add_item(2, [1, 1, 0])
    p a.build(1)
    p a.get_nns_by_item(0, 10)
    p a.get_nns_by_vector([0, 1, 1], 2)
    p a.get_item(0)
    p a.get_distance(0, 1)
    p a.n_items
    p a.n_trees
    p a.save('test.dat')
    p a.load('test.dat')
    p a.n_items
  end
end
