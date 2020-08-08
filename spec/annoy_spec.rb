# frozen_string_literal: true

RSpec.describe Annoy do
  it 'has a version number' do
    expect(Annoy::VERSION).not_to be nil
  end

  describe Annoy::AnnoyIndex do
    let(:n_features) { 4 }
    let(:metric) { 'manhattan' }
    let(:n_trees) { 3 }
    let(:n_items) { 5 }
    let(:n_neighbors) { 3 }
    let(:index) { Annoy::AnnoyIndex.new(n_features: n_features, metric: metric) }

    before do
      index.seed(1)
      index.add_item(0, [0, 1, 2, 3])
      index.add_item(1, [1, 1, 2, 3])
      index.add_item(2, [1, 1, 2, 4])
      index.add_item(3, [2, 1, 2, 5])
      index.add_item(4, [2, 1, 2, 6])
      index.build(n_trees)
    end

    describe '#get_nns_by_item' do
      context 'when include_distances is false' do
        subject { index.get_nns_by_item(0, n_neighbors) }

        it 'returns id list of nearest neighbors' do
          expect(subject).to match([0, 1, 2])
        end
      end

      context 'when include_distances is true' do
        subject { index.get_nns_by_item(0, n_neighbors, include_distances: true) }

        it 'returns id list and distances of nearest neighbors' do
          expect(subject).to match([[0, 1, 2], [0, 1, 2]])
        end
      end
    end

    describe '#get_nns_by_vector' do
      let(:query) { [0, 1, 2, 3] }

      context 'when include_distances is false' do
        subject { index.get_nns_by_vector(query, n_neighbors) }

        it 'returns id list of nearest neighbors' do
          expect(subject).to match([0, 1, 2])
        end
      end

      context 'when include_distances is true' do
        subject { index.get_nns_by_vector(query, n_neighbors, include_distances: true) }

        it 'returns id list and distances of nearest neighbors' do
          expect(subject).to match([[0, 1, 2], [0, 1, 2]])
        end
      end
    end

    describe '#get_item' do
      subject { index.get_item(0) }

      it 'returns the item vector specified by ID' do
        expect(subject).to match([0, 1, 2, 3])
      end
    end

    describe '#n_fetures' do
      subject { index.n_features }

      it 'returns the number of features' do
        expect(subject).to eq(n_features)
      end
    end

    describe '#metric' do
      subject { index.metric }

      it 'returns the distance metric' do
        expect(subject).to eq(metric)
      end
    end

    describe '#n_items' do
      subject { index.n_items }

      it 'returns the number of indexed items' do
        expect(subject).to eq(n_items)
      end
    end

    describe '#n_trees' do
      subject { index.n_trees }

      it 'returns the number of index trees' do
        expect(subject).to eq(n_trees)
      end
    end

    describe '#save and #load' do
      let(:filename) { File.expand_path(__dir__ + '/test.ann') }

      before { index.save(filename) }

      it 'load the saved index', :aggregate_failures do
        loaded = Annoy::AnnoyIndex.new(n_features: n_features, metric: metric)
        loaded.load(filename)
        expect(loaded.n_items).to eq(index.n_items)
        expect(loaded.n_trees).to eq(index.n_trees)
        expect(loaded.get_item(0)).to match(index.get_item(0))
      end
    end

    describe '#get_distance' do
      subject { index.get_distance(0, 1) }

      before do
        index.unload
        index.add_item(0, [1, 0, 0, 1])
        index.add_item(1, [0, 1, 0, 1])
      end

      context "when metric is 'angular'" do
        let(:metric) { 'angular' }

        it 'returns cosine distance between items' do
          expect(subject).to eq(1)
        end
      end

      context "when metric is 'dot'" do
        let(:metric) { 'dot' }

        it 'returns dot product distance between items' do
          expect(subject).to eq(1)
        end
      end

      context "when metric is 'hamming'" do
        let(:metric) { 'hamming' }

        it 'returns hamming distance between items' do
          expect(subject).to eq(2)
        end
      end

      context "when metric is 'euclidean'" do
        let(:metric) { 'euclidean' }

        it 'returns euclidean distance between items' do
          expect(subject).to be_within(1e-8).of(1.41421356)
        end
      end

      context "when metric is 'manhattan'" do
        let(:metric) { 'manhattan' }

        it 'returns manhattan distance between items' do
          expect(subject).to eq(2)
        end
      end
    end
  end
end
